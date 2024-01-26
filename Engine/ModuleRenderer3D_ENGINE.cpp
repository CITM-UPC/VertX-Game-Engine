#include "ModuleGameEngine.h"
#include "Globals_ENGINE.h"
#include "ModuleRenderer3D_ENGINE.h"
#include <GL/glew.h>
#include "SDL2/SDL_opengl.h"
#include <IL/il.h>
#include <iostream>

ModuleRenderer3D_ENGINE::ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled) : Engine_Module(game_engine, start_enabled)
{
	vsync = false;
	screen_width = 1024;
	screen_height = 768;
}

// Destructor
ModuleRenderer3D_ENGINE::~ModuleRenderer3D_ENGINE() = default;

// Called before render is available
bool ModuleRenderer3D_ENGINE::Init()
{
	bool ret = true;

	if (targetWindow == nullptr)
	{
		/*LOG("ENGINE: Target window has not been set. Try initializing the variable with 'SetTargetWindow()'", NULL);*/
		ret = false;
	}

	//Create context
	context = SDL_GL_CreateContext(targetWindow);
	if (context == nullptr)
	{
		/*LOG("ENGINE: OpenGL context could not be created!", NULL);*/
		ret = false;
	}

	if (ret == true)
	{
		// Initialize DevIL
		/*LOG("[Engine] Initializing DevIL");*/
		ilInit();

		// Initialize glew
		/*LOG("[Engine] Initializing OpenGL", NULL);*/
		glewInit();

		//Use Vsync
		if (vsync && SDL_GL_SetSwapInterval(1) < 0)
			/*LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());*/

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			/*LOG("Error initializing OpenGL! %s\n", gluErrorString(error));*/
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			/*LOG("Error initializing OpenGL! %s\n", gluErrorString(error));*/
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			/*LOG("Error initializing OpenGL! %s\n", gluErrorString(error));*/
			ret = false;
		}

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		depth_test = true;

		glEnable(GL_CULL_FACE);
		cull_face = true;

		glEnable(GL_COLOR_MATERIAL);
		color_material = true;

		//glEnable(GL_LIGHTING);
		lighting = false;

		line_smooth = false;

		polygon_smooth = false;
	}

	debugRayCast = false;

	// Projection matrix for
	OnResize(screen_width, screen_height);

	return ret;
}

// PreUpdate: clear buffer
engine_status ModuleRenderer3D_ENGINE::PreUpdate()
{
	game_engine->cameraGO.UpdateComponents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(game_engine->cameraGO.GetComponent<Camera>()->fov,
		game_engine->cameraGO.GetComponent<Camera>()->aspectRatio,
		game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear,
		game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	game_engine->cameraGO.GetComponent<Camera>()->lookAtPos = game_engine->cameraGO.GetComponent<Transform>()->position() + game_engine->cameraGO.GetComponent<Transform>()->forward() * game_engine->cameraGO.GetComponent<Camera>()->camOffset;

	gluLookAt(game_engine->cameraGO.GetComponent<Transform>()->position().x, game_engine->cameraGO.GetComponent<Transform>()->position().y, game_engine->cameraGO.GetComponent<Transform>()->position().z,
		game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.x, game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.y, game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.z,
		game_engine->cameraGO.GetComponent<Transform>()->up().x, game_engine->cameraGO.GetComponent<Transform>()->up().y, game_engine->cameraGO.GetComponent<Transform>()->up().z);

	return ENGINE_UPDATE_CONTINUE;
}

engine_status ModuleRenderer3D_ENGINE::Update()
{
	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_status ModuleRenderer3D_ENGINE::PostUpdate()
{
	//Temporary, only debug

	if (debugRayCast)
	{
		glLineWidth(1.0);
		glBegin(GL_LINES);

		double size = 0.05;
		for (int i = 0; i < origins.size(); i++)
		{
			glColor3ub(0, 255, 0);
			glVertex3d(camPos[i].x, camPos[i].y, camPos[i].z - size);
			glVertex3d(camPos[i].x, camPos[i].y, camPos[i].z + size);
			glVertex3d(camPos[i].x, camPos[i].y - size, camPos[i].z);
			glVertex3d(camPos[i].x, camPos[i].y + size, camPos[i].z);
			glVertex3d(camPos[i].x - size, camPos[i].y, camPos[i].z);
			glVertex3d(camPos[i].x + size, camPos[i].y, camPos[i].z);

			glColor3ub(255, 255, 0);
			glVertex3d(origins[i].x, origins[i].y, origins[i].z);
			glColor3ub(255, 255, 255);
			glVertex3d(ends[i].x, ends[i].y, ends[i].z);
		}

		for (int i = 0; i < nearPlanes.size() / 4; i++)
		{
			glColor3ub(50, 150, 255);
			glVertex3d(nearPlanes[i].x, nearPlanes[i].y, nearPlanes[i].z);
			glVertex3d(nearPlanes[i + 1].x, nearPlanes[i + 1].y, nearPlanes[i + 1].z);

			glVertex3d(nearPlanes[i + 1].x, nearPlanes[i + 1].y, nearPlanes[i + 1].z);
			glVertex3d(nearPlanes[i + 2].x, nearPlanes[i + 2].y, nearPlanes[i + 2].z);

			glVertex3d(nearPlanes[i + 2].x, nearPlanes[i + 2].y, nearPlanes[i + 2].z);
			glVertex3d(nearPlanes[i + 3].x, nearPlanes[i + 3].y, nearPlanes[i + 3].z);

			glVertex3d(nearPlanes[i + 3].x, nearPlanes[i + 3].y, nearPlanes[i + 3].z);
			glVertex3d(nearPlanes[i].x, nearPlanes[i].y, nearPlanes[i].z);
		}

		glEnd();
	}

	/*GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}
	assert(error == GL_NO_ERROR);*/

	return ENGINE_UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D_ENGINE::CleanUp()
{
	//ENGINE_LOG("Destroying 3D Renderer");

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
	game_engine->cameraGO.GetComponent<Camera>()->aspectRatio = (double)width / (double)height;
	ProjectionMatrix = glm::perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(glm::value_ptr(ProjectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D_ENGINE::DrawGrid(int size, int step, bool xzAxis, bool xyAxis, bool zyAxis)
{
	glLineWidth(1.0);
	glColor3ub(128, 128, 128);

	glBegin(GL_LINES);
	for (int i = -size; i <= size; i += step) {
		if (xzAxis)
		{
			//XZ plane
			glVertex3i(i, 0, -size);
			glVertex3i(i, 0, size);
			glVertex3i(-size, 0, i);
			glVertex3i(size, 0, i);
		}
		if (xyAxis)
		{
			//XY plane
			glVertex2i(i, -size);
			glVertex2i(i, size);
			glVertex2i(-size, i);
			glVertex2i(size, i);
		}
		if (zyAxis)
		{
			//ZY plane
			glVertex3i(0, i, -size);
			glVertex3i(0, i, size);
			glVertex3i(0, -size, i);
			glVertex3i(0, size, i);
		}
	}
	glEnd();

	//drawCubeTest();

	drawAxis();
}

void ModuleRenderer3D_ENGINE::SwapDepthTest()
{
	if (depth_test) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D_ENGINE::SwapCullFace()
{
	if (cull_face) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D_ENGINE::SwapLighting()
{
	if (lighting) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D_ENGINE::SwapColorMaterial()
{
	if (color_material) {
		glEnable(GL_COLOR_MATERIAL);
	}
	else {
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D_ENGINE::SwapLineSmooth()
{
	if (line_smooth) {
		glEnable(GL_LINE_SMOOTH);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

void ModuleRenderer3D_ENGINE::SwapPolygonSmooth()
{
	if (polygon_smooth) {
		glEnable(GL_POLYGON_SMOOTH);
	}
	else {
		glDisable(GL_POLYGON_SMOOTH);
	}
}