#include "ModuleGameEngine.h"
#include "Globals_ENGINE.h"
#include "..\Editor\Globals.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <IL/il.h>


ModuleRenderer3D_ENGINE::ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled) : Engine_Module(game_engine, start_enabled)
{
	vsync = VSYNC;
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
			LOG("Failed to create the directory:");
		}
	}
}

// Function to render folders and files in ImGui
void ModuleRenderer3D_ENGINE::HandleFileDrop(const char* filePath) {
	// Extract the file name and extension
	LOG("Dropped file: %s", filePath);
	// Extract the filename from the file path
	const char* lastBackslash = strrchr(filePath, '\\');  // Use backslash for Windows
	std::string filename = (lastBackslash) ? lastBackslash + 1 : filePath;

	// Determine the destination subfolder based on the file type
	const char* subfolder = nullptr;
	const char* extension = strrchr(filename.c_str(), '.');
	if (extension) {
		if (_stricmp(extension, ".fbx") == 0) {
			subfolder = fbxAssetsDirectory;
		}
		else if (_stricmp(extension, ".png") == 0 || _stricmp(extension, ".jpg") == 0 || _stricmp(extension, ".jpeg") == 0) {
			subfolder = imageAssetsDirectory;
		}
	}

	if (subfolder) {
		CreateDirectoryIfNotExists(parentDirectory); // Ensure the parent "Assets" directory exists
		CreateDirectoryIfNotExists(subfolder); // Create the subfolder if it doesn't already exist

	// Combine the target directory with the filename to create the destination path
		std::string destinationPath = subfolder;
		destinationPath += "\\";
		destinationPath += filename;

		// Copy the file to the appropriate subfolder
		if (CopyFileA(filePath, destinationPath.c_str(), FALSE)) {
			std::cout << "File copied to: " << destinationPath << std::endl;
		}
		else {
			std::cerr << "Failed to copy the file. Error code: " << GetLastError() << std::endl;
		}
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

	// Initialize Glew
	GLenum error = glewInit();
	if (GLEW_OK != error)
	{
		LOG("ENGINE: Error initializing Glew! %s\n", glewGetErrorString(error));
	}
	LOG("ENGINE Status: Using Glew version %s\n", glewGetString(GLEW_VERSION))


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

	// Initialize DevIL
	ilInit();

	OnResize(screen_width, screen_height);

	addFbx("Assets/BakerHouse.fbx");


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

	// Vsync toggle
	if (vsync)
	{	
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}


	return ENGINE_UPDATE_CONTINUE;
}

engine_update_status ModuleRenderer3D_ENGINE::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			// Handle quit event
			break;
		case SDL_DROPFILE:
			LOG("File Dropped");
			// Handle file drop event
			HandleFileDrop(event.drop.file);
			SDL_free(event.drop.file); // Free the dropped file
			break;
			// Add other event handling as needed
		}
	}

	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_update_status ModuleRenderer3D_ENGINE::PostUpdate()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (const auto& vector : meshList) {
		for (const auto& mesh_ptr : vector) {
			mesh_ptr->draw();
		}
	}

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