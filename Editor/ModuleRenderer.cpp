#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*X = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	Y = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);
	Z = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);*/
}

// Destructor
ModuleRenderer::~ModuleRenderer()
{}

// Called before render is available
bool ModuleRenderer::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	ret = App->gEngine->renderer3D_engine->Init();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer::PreUpdate()
{
	App->gEngine->renderer3D_engine->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update()
{
	// Compute camera movement based on player's input so it can be later updated by the renderer
	FreeCameraMovement();

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		App->gEngine->cam.ResetCameraParameters();

	App->gEngine->renderer3D_engine->Update();

	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer::PostUpdate()
{
	// Drawing XYZ axis + XZ grid
	App->gEngine->renderer3D_engine->DrawAxis(4.0f);
	App->gEngine->renderer3D_engine->DrawGrid(100, 1, true);

	// We render first the Engine's Renderer 3D into the Editor's Renderer
	App->gEngine->renderer3D_engine->PostUpdate();

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

	App->gEngine->renderer3D_engine->CleanUp();

	return true;
}


void ModuleRenderer::FreeCameraMovement()
{
	double cameraSpeed = 0.002;	
	float cameraSpeedMultiplier = 3.0f;

	// Normalized vector of the substraction of the Camera Focus Vec and the Camera Position Vec, then multiplied by the speed we want
	vec3 normalizedVec = (cameraSpeed * (glm::normalize(App->gEngine->cam.lookAtPos - App->gEngine->cam.camCenterPos)));


	// Rotation matrix definition
	mat3 rotationMat = mat3(cos(glm::radians(90.0f)), 0, sin(glm::radians(90.0f)),
										0,			  1,		   0,
						   -sin(glm::radians(90.0f)), 0, cos(glm::radians(90.0f)));

	// Normal vector between the rotation matrix and the normalized vector
	vec3 normalVec = rotationMat * normalizedVec;
	

	/* KEYBOARD CAMERA MOVEMENT */
	// Compute camera movement if 'MOUSE RIGHT CLICK' is pressed
	if (App->input->GetMouseButton(3))
	{
		// Check if 'LEFT SHIFT' is pressed, then camera speed is higher (x3) times
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			normalizedVec *= cameraSpeedMultiplier;
			normalVec *= cameraSpeedMultiplier;
		}

		// Check if 'W' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			// Compute FORWARD camera movement
			App->gEngine->cam.camCenterPos += normalizedVec;
			App->gEngine->cam.lookAtPos += normalizedVec;
		}
		// Check if 'S' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			// Compute BACKWARDS camera movement
			App->gEngine->cam.camCenterPos -= normalizedVec;
			App->gEngine->cam.lookAtPos -= normalizedVec;
		}

		// Check if 'A' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			// Compute LEFT camera movement
			normalVec.y = 0; // Set to zero the Y component, otherwise it gets f*ucked idk why tbh
			App->gEngine->cam.lookAtPos -= normalVec;
			App->gEngine->cam.camCenterPos -= normalVec;
		}

		// Check if 'D' key is pressed
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			// Compute RIGHT camera movement
			normalVec.y = 0;	
			App->gEngine->cam.lookAtPos += normalVec;
			App->gEngine->cam.camCenterPos += normalVec;
		}
		
	}



	/* MOUSE CAMERA MOVEMENT */
	
	/*if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		App->gEngine->cam.camCenterPos -= App->gEngine->cam.lookAtPos;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = glm::rotate(X)

			X = glm::rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = glm::rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = glm::rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = glm::rotate(Y, DeltaY, X);
			Z = glm::rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = glm::cross(Z, X);
			}
		}

		App->gEngine->cam.camCenterPos = App->gEngine->cam.lookAtPos + Z * length(App->gEngine->cam.camCenterPos);
	}
	*/

	
}