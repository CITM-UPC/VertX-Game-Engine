#include "ModuleGameEngine.h"
#include "Globals_ENGINE.h"
#include "..\Editor\Globals.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Mesh.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <IL/il.h>


ModuleRenderer3D_ENGINE::ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled) : Engine_Module(game_engine, start_enabled)
{
	vsync = false;
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;
}

// Destructor
ModuleRenderer3D_ENGINE::~ModuleRenderer3D_ENGINE()
{}

void ModuleRenderer3D_ENGINE::CreateDirectoryIfNotExists(const char* directory) {
	// Create the directory if it doesn't exist
	if (!CreateDirectoryA(directory, nullptr)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			std::cerr << "Failed to create the directory: " << directory << std::endl;
		}
	}
}


void ModuleRenderer3D_ENGINE::HandleFileDrop(const std::string& filePath, Folder& rootFolder) {
	// Extract the file name and extension
	size_t lastSlashPos = filePath.find_last_of("/\\");
	std::string fileName = (lastSlashPos != std::string::npos) ? filePath.substr(lastSlashPos + 1) : filePath;
	size_t lastDotPos = fileName.find_last_of(".");
	std::string fileExtension = (lastDotPos != std::string::npos) ? fileName.substr(lastDotPos) : "";

	// Determine the target folder based on the file extension
	std::string targetFolderName = "Other"; // Default folder
	if (fileExtension == ".fbx") {
		targetFolderName = "FBX_Assets";
	}
	else if (fileExtension == ".png" || fileExtension == ".jpg" || fileExtension == ".jpeg") {
		targetFolderName = "Image_Assets";
	}

	// Find or create the target folder
	Folder* targetFolder = nullptr;
	for (auto& folder : rootFolder.subfolders) {
		if (folder.name == targetFolderName) {
			targetFolder = &folder;
			break;
		}
	}

	if (!targetFolder) {
		Folder newFolder;
		newFolder.name = targetFolderName;
		rootFolder.subfolders.push_back(newFolder);
		targetFolder = &rootFolder.subfolders.back();
	}

	// Add the file to the target folder
	targetFolder->files[fileExtension].push_back(fileName);
}

// Function to add a dynamically created folder
void ModuleRenderer3D_ENGINE::AddDynamicFolder(Folder& parentFolder, const std::string& folderName) {
	Folder newFolder;
	newFolder.name = folderName;
	parentFolder.subfolders.push_back(newFolder);
}

// Function to render folders and files in ImGui
void ModuleRenderer3D_ENGINE::RenderFoldersAndFiles(const Folder& folder) {
	if (ImGui::TreeNode(folder.name.c_str())) {
		for (const auto& subfolder : folder.subfolders) {
			RenderFoldersAndFiles(subfolder);
		}

		for (const auto& file : folder.files) {
			if (!file.second.empty()) {
				ImGui::Text("Files with %s extension:", file.first.c_str());
				ImGui::Indent();
				for (const auto& fileName : file.second) {
					ImGui::Text("%s", fileName.c_str());
				}
				ImGui::Unindent();
			}
		}

		ImGui::TreePop();
	}
}

bool ModuleRenderer3D_ENGINE::CleanUpAssets() {
	// Close any open handles to files or directories, if applicable
	// CloseHandlesToAssets();

	// Clean up the parent "Assets" directory, including subfolders and files
	if (RecursiveRemoveDirectory(parentDirectory)) {
		std::cout << "Assets directory cleaned up." << std::endl;
		return true;
	}
	else {
		std::cerr << "Failed to clean up assets directory." << std::endl;
		return false;
	}
}

bool ModuleRenderer3D_ENGINE::RecursiveRemoveDirectory(const char* directory) {

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(std::string(directory).append("\\*").c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		return true;
	}

	bool success = true;

	do {
		if (findFileData.cFileName[0] != '.') {
			std::string fullPath = std::string(directory).append("\\").append(findFileData.cFileName);
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// Recursively remove subdirectories
				if (!RecursiveRemoveDirectory(fullPath.c_str())) {
					success = false;
				}
			}
			else {
				// Remove read-only attribute if set
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
					SetFileAttributes(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
				}
				// Delete files
				if (DeleteFile(fullPath.c_str()) == 0) {
					std::cerr << "Failed to delete file: " << fullPath << " Error code: " << GetLastError() << std::endl;
					success = false;
				}
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);

	// Remove read-only attribute if set for the directory itself
	if (GetFileAttributes(directory) & FILE_ATTRIBUTE_READONLY) {
		SetFileAttributes(directory, FILE_ATTRIBUTE_NORMAL);
	}

	// Delete the directory
	if (RemoveDirectory(directory) == 0) {
		std::cerr << "Failed to delete directory: " << directory << " Error code: " << GetLastError() << std::endl;
		success = false;
	}

	return success;
}

// Called before render is available
bool ModuleRenderer3D_ENGINE::Init()
{
	LOG_("ENGINE: Creating 3D Renderer context");
	bool ret = true;

	if (targetWindow == NULL)
	{
		LOG_("ENGINE: Target window has not been set. Try initializing the variable with 'SetTargetWindow()'");
		ret = false;
	}

	//Create context
	context = SDL_GL_CreateContext(targetWindow);
	if (context == NULL)
	{
		LOG_("ENGINE: OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (vsync && SDL_GL_SetSwapInterval(1) < 0)
			LOG_("ENGINE: Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_("ENGINE: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_("ENGINE: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

		//Check for an error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_("ENGINE: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHTING);
	}

	OnResize(screen_width, screen_height);

	ilInit();

	return ret;
}

// PreUpdate: clear buffer
engine_update_status ModuleRenderer3D_ENGINE::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(game_engine->camera.fov, game_engine->camera.aspectRatio, game_engine->camera.clippingPlaneViewNear, game_engine->camera.clippingPlaneViewFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(game_engine->camera.worldPosVec.x, game_engine->camera.worldPosVec.y, game_engine->camera.worldPosVec.z,
			game_engine->camera.focusPosVec.x, game_engine->camera.focusPosVec.y, game_engine->camera.focusPosVec.z,
			game_engine->camera.upVec.x , game_engine->camera.upVec.y , game_engine->camera.upVec.z);
	
	
	viewMatrix = glm::lookAt(game_engine->camera.worldPosVec,
							game_engine->camera.focusPosVec,
							game_engine->camera.upVec);

	glLoadMatrixf(glm::value_ptr(viewMatrix));


	return ENGINE_UPDATE_CONTINUE;
}

engine_update_status ModuleRenderer3D_ENGINE::Update()
{
	Folder rootFolder;
	rootFolder.name = "Assets";

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			// Handle quit event
			break;
		case SDL_DROPFILE:
			LOG("File Dropped");
			// Handle file drop event
			HandleFileDrop(event.drop.file, rootFolder);
			SDL_free(event.drop.file); // Free the dropped file
			break;
			// Add other event handling as needed
		}
	}

	RenderFoldersAndFiles(rootFolder);

	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_update_status ModuleRenderer3D_ENGINE::PostUpdate()
{
#pragma region Baked House

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*static auto mesh_ptrs = Mesh::loadFromFile("BakerHouse.fbx");
	for (auto& mesh_ptr : mesh_ptrs) mesh_ptr->draw();*/

#pragma endregion

	return ENGINE_UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D_ENGINE::CleanUp()
{

	LOG_("ENGINE: Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	targetWindow = nullptr;
	delete targetWindow;

	// Call the CleanupAssets function to clean up asset directories
	CleanUpAssets();

	return true;
}


void ModuleRenderer3D_ENGINE::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = glm::perspective(70.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D_ENGINE::DrawGrid(int size, int step, bool xzAxis)
{
	glLineWidth(1.0);
    glColor3ub(128, 128, 128);	// Grey Color

    glBegin(GL_LINES);
    for (int i = -size; i <= size; i += step) {

		if (xzAxis)
		{
			//XZ grid plane
			glVertex3i(i, 0, -size);
			glVertex3i(i, 0, size);
			glVertex3i(-size, 0, i);
			glVertex3i(size, 0, i);
		}
    }
    glEnd();
}

void ModuleRenderer3D_ENGINE::DrawAxis(float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	
	// X
	glColor3ub(255, 0, 0);	// Red
	glVertex3d(0, 0, 0);
	glVertex3d(1.0, 0, 0);

	// Y
	glColor3ub(0, 255, 0);	// Green
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1.0, 0);
	
	// Z
	glColor3ub(0, 0, 200);	// Blue
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1.0);
	
	glEnd();
}