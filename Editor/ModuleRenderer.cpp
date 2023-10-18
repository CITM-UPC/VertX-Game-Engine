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
