#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

// Destructor
ModuleRenderer::~ModuleRenderer()
{}

// Called before render is available
bool ModuleRenderer::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	ret = App->game_engine->renderer3D_engine->Init();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer::PreUpdate()
{
	App->game_engine->renderer3D_engine->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update()
{
	// Compute camera movement based on player's input so it can be later updated by the renderer
	FreeCameraMovement();

	// Compute camera orbitation around an specific world point (LALT + Left Click)
	CameraOrbitation();


	// Function to reset camera parameters to the initial ones
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		App->game_engine->camera.ResetCameraParameters();


	App->game_engine->renderer3D_engine->Update();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer::PostUpdate()
{
	// Drawing XYZ axis + XZ grid
	App->game_engine->renderer3D_engine->DrawAxis(4.0f);
	App->game_engine->renderer3D_engine->DrawGrid(200, 1);


	// We render first the Engine's Renderer 3D into the Editor's Renderer
	App->game_engine->renderer3D_engine->PostUpdate();

	// Now we render the ImGUI stuff after the Engine's Renderer 3D
	App->imgui->RenderImGUI();


	// Updating/Swapping the window with OpenGL rendering. Used in a double-buffered OpenGL context
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer::CleanUp()
{
	LOG("Destroying Renderer");


	App->game_engine->renderer3D_engine->CleanUp();

	return true;
}

void ModuleRenderer::CameraZoomIn()
{
	vec3 normalizedVec = glm::normalize(App->game_engine->camera.focusPosVec - App->game_engine->camera.worldPosVec);

	// Update worldPosVec from gluLookAt(), which represents the position of the camera viewport
	App->game_engine->camera.worldPosVec.x += App->game_engine->camera.zoomSpeed * normalizedVec.x;
	App->game_engine->camera.worldPosVec.y += App->game_engine->camera.zoomSpeed * normalizedVec.y;
	App->game_engine->camera.worldPosVec.z += App->game_engine->camera.zoomSpeed * normalizedVec.z;

	// Update focusPosVec from gluLookAt(), which represents the focus point where the camera is looking at
	App->game_engine->camera.focusPosVec.x += App->game_engine->camera.zoomSpeed * normalizedVec.x;
	App->game_engine->camera.focusPosVec.y += App->game_engine->camera.zoomSpeed * normalizedVec.y;
	App->game_engine->camera.focusPosVec.z += App->game_engine->camera.zoomSpeed * normalizedVec.z;
}

void ModuleRenderer::CameraZoomOut()
{
	vec3 normalizedVec = glm::normalize(App->game_engine->camera.focusPosVec - App->game_engine->camera.worldPosVec);

	// Update worldPosVec from gluLookAt(), which represents the position of the camera viewport
	App->game_engine->camera.worldPosVec.x -= App->game_engine->camera.zoomSpeed * normalizedVec.x;
	App->game_engine->camera.worldPosVec.y -= App->game_engine->camera.zoomSpeed * normalizedVec.y;
	App->game_engine->camera.worldPosVec.z -= App->game_engine->camera.zoomSpeed * normalizedVec.z;

	// Update focusPosVec from gluLookAt(), which represents the focus point where the camera is looking at
	App->game_engine->camera.focusPosVec.x -= App->game_engine->camera.zoomSpeed * normalizedVec.x;
	App->game_engine->camera.focusPosVec.y -= App->game_engine->camera.zoomSpeed * normalizedVec.y;
	App->game_engine->camera.focusPosVec.z -= App->game_engine->camera.zoomSpeed * normalizedVec.z;
}

void ModuleRenderer::CameraOrbitation()
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		// Setting the Camera Focus Point to the desired
		App->game_engine->camera.focusPosVec.x = 0;
		App->game_engine->camera.focusPosVec.y = 0;
		App->game_engine->camera.focusPosVec.z = 0;

		double radius = glm::length(App->game_engine->camera.worldPosVec - App->game_engine->camera.focusPosVec);

		// Get mouse motion input from user
		float deltaX = (App->input->GetMouseXMotion() / 2);

		float angleChange = glm::radians(deltaX);

		// Calculate new camera position
		glm::dvec3 relativePos = App->game_engine->camera.worldPosVec - App->game_engine->camera.focusPosVec;

		mat3 rotationMat = mat3(cos(angleChange), 0, sin(angleChange),
										0,		  1,		0,
							   -sin(angleChange), 0, cos(angleChange));

		vec3 rotatedRelativePos = rotationMat * relativePos;
		App->game_engine->camera.worldPosVec = App->game_engine->camera.focusPosVec + rotatedRelativePos;

		// Maintain the distance (radius) from the origin
		App->game_engine->camera.worldPosVec = App->game_engine->camera.focusPosVec + (glm::normalize(App->game_engine->camera.worldPosVec - App->game_engine->camera.focusPosVec) * radius);
	}
}

void ModuleRenderer::FreeCameraMovement()
{
	// Normalized vector of the substraction of the Camera Focus Vec and the Camera Position Vec, then multiplied by the speed we want
	vec3 normalizedVec = (App->game_engine->camera.cameraSpeed * (glm::normalize(App->game_engine->camera.focusPosVec - App->game_engine->camera.worldPosVec)));


	// Rotation matrix definition
	mat3 rotationMat = mat3(cos(glm::radians(90.0f)), 0, sin(glm::radians(90.0f)),
										0,			  1,		   0,
						   -sin(glm::radians(90.0f)), 0, cos(glm::radians(90.0f)));

	// Normal vector between the rotation matrix and the normalized vector
	vec3 normalVec = rotationMat * normalizedVec;
	

	/* KEYBOARD CAMERA MOVEMENT */
	// Compute camera movement if 'MOUSE RIGHT CLICK' is pressed
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		// Check if 'LEFT SHIFT' is pressed, then camera speed is higher (x3) times
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			normalizedVec *= App->game_engine->camera.cameraSpeedMultiplier;
			normalVec *= App->game_engine->camera.cameraSpeedMultiplier;
		}

		// Check if 'W' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			// Compute FORWARD camera movement
			App->game_engine->camera.worldPosVec += normalizedVec;
			App->game_engine->camera.focusPosVec += normalizedVec;
		}
		// Check if 'S' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			// Compute BACKWARDS camera movement
			App->game_engine->camera.worldPosVec -= normalizedVec;
			App->game_engine->camera.focusPosVec -= normalizedVec;
		}

		// Check if 'A' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			// Compute LEFT camera movement
			normalVec.y = 0; // Set to zero the Y component, otherwise it gets f*ucked idk why tbh
			App->game_engine->camera.worldPosVec -= normalVec;
			App->game_engine->camera.focusPosVec -= normalVec;
		}

		// Check if 'D' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			// Compute RIGHT camera movement
			normalVec.y = 0;	
			App->game_engine->camera.worldPosVec += normalVec;
			App->game_engine->camera.focusPosVec += normalVec;
		}
		

		/* MOUSE CAMERA MOVEMENT */
		// Compute mouse input displacement
		int deltaX = App->input->GetMouseXMotion();
		int deltaY = -App->input->GetMouseYMotion();

		App->game_engine->camera.cameraYaw += deltaX * App->game_engine->camera.mouseSensitivity;
		App->game_engine->camera.cameraPitch += deltaY * App->game_engine->camera.mouseSensitivity;

		// Limiting Camera Piitch to prevent flipping
		if (App->game_engine->camera.cameraPitch > 89.0f)
			App->game_engine->camera.cameraPitch = 89.0f;
		if (App->game_engine->camera.cameraPitch < -89.0f)
			App->game_engine->camera.cameraPitch = -89.0f;

		glm::dvec3 directionVec;
		directionVec.x = cos(glm::radians(App->game_engine->camera.cameraYaw)) * cos(glm::radians(App->game_engine->camera.cameraPitch));
		directionVec.y = sin(glm::radians(App->game_engine->camera.cameraPitch));
		directionVec.z = sin(glm::radians(App->game_engine->camera.cameraYaw)) * cos(glm::radians(App->game_engine->camera.cameraPitch));

		// Update Camera's Focus view point vector to be recomputed in the renderer with gluLookAt()
		App->game_engine->camera.focusPosVec = App->game_engine->camera.worldPosVec + directionVec;
	}
}