#include "ModuleGameEngine.h"
#include "Globals_ENGINE.h"
#include "..\Editor\Globals.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include <string>
#include <iostream>
#include <fstream>


ModuleRenderer3D_ENGINE::ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled) : Engine_Module(game_engine, start_enabled)
{
	vsync = false;
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;
}

// Destructor
ModuleRenderer3D_ENGINE::~ModuleRenderer3D_ENGINE()
{}


void ModuleRenderer3D_ENGINE::HandleFileDrop(const char* filePath) {
	// Log the dropped filename using your custom LOG() function or printf
	LOG("Dropped file: %s", filePath);

	// Define the target directory within your project
	const char* targetDirectory = "FBX_Assets";

	// Extract the filename from the file path
	const char* lastBackslash = strrchr(filePath, '\\');  // Use backslash for Windows
	std::string filename = (lastBackslash) ? lastBackslash + 1 : filePath;

	// Combine the target directory with the filename
	std::string destinationPath = targetDirectory;
	destinationPath += "\\"; // Use backslashes for Windows
	destinationPath += filename;

	// Create the target directory if it doesn't exist
	if (!CreateDirectory(destinationPath.c_str(), NULL)) {
		LOG("Failed to create the target directory: %s", targetDirectory);
		return;
	}

	// Copy the file
	if (CopyFileA(filePath, destinationPath.c_str(), FALSE)) {
		LOG("File copied to: %s", destinationPath.c_str());
	}
	else {
		LOG("Failed to copy the file. Error code: %d", GetLastError());
	}
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
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
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
#pragma region TriangleTest

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Triangle for testing
	/*glColor4ub(255, 0, 0, 255);
	glBegin(GL_TRIANGLES);
	glVertex3d(-0.25, 0, 0);
	glVertex3d(0.25, 0, 0);
	glVertex3d(0, 0.5, 0);
	glEnd();*/



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