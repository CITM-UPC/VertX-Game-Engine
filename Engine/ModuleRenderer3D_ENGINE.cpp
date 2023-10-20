#include "GameEngine.h"
#include "Globals_ENGINE.h"
#include "..\Editor\Globals.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"

ModuleRenderer3D_ENGINE::ModuleRenderer3D_ENGINE(GameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
	vsync = false;
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;
}

// Destructor
ModuleRenderer3D_ENGINE::~ModuleRenderer3D_ENGINE()
{}

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
engine_status ModuleRenderer3D_ENGINE::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(gEngine->cam.fov, gEngine->cam.aspectRatio, gEngine->cam.clippingPlaneViewNear, gEngine->cam.clippingPlaneViewFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gEngine->cam.camCenterPos.x, gEngine->cam.camCenterPos.y,	gEngine->cam.camCenterPos.z,
			  gEngine->cam.lookAtPos.x, gEngine->cam.lookAtPos.y, gEngine->cam.lookAtPos.z,
			  gEngine->cam.upVector.x , gEngine->cam.upVector.y , gEngine->cam.upVector.z);
	
	
	viewMatrix = glm::lookAt(gEngine->cam.camCenterPos,
							 gEngine->cam.lookAtPos,
							 gEngine->cam.upVector);

	glLoadMatrixf(glm::value_ptr(viewMatrix));


	return ENGINE_UPDATE_CONTINUE;
}

engine_status ModuleRenderer3D_ENGINE::Update()
{


	return ENGINE_UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
engine_status ModuleRenderer3D_ENGINE::PostUpdate()
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

void ModuleRenderer3D_ENGINE::DrawGrid(int size, int step, bool xzAxis, bool xyAxis, bool zyAxis)
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
		if (xyAxis)
		{
			//XY grid plane
			glVertex2i(i, -size);
			glVertex2i(i, size);
			glVertex2i(-size, i);
			glVertex2i(size, i);
		}
		if (zyAxis)
		{
			//ZY grid plane
			glVertex3i(0, i, -size);
			glVertex3i(0, i, size);
			glVertex3i(0, -size, i);
			glVertex3i(0, size, i);
		}
    }
    glEnd();
}

void ModuleRenderer3D_ENGINE::DrawAxis(float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	
	glColor3ub(255, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(1.0, 0, 0);

	glColor3ub(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1.0, 0);
	
	glColor3ub(0, 0, 200);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1.0);
	
	glEnd();
}