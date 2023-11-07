#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("EDITOR: Init SDL window & surface", NULL);
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("EDITOR: SDL_VIDEO could not initialize! ERROR: %s", SDL_GetError());

		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("EDITOR: Window could not be created! ERROR: %s", SDL_GetError());

			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}

		App->game_engine->renderer3D_engine->SetTargetWindow(window);
	}

	return ret;
}

update_status ModuleWindow::Update()
{
	// Enable/Disable window fullscreen mode
	if (fullscreenEnabled)
	{
		if (fullcreenDesktopEnabled)
		{
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else
		{
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		}
	}
	else
	{
		SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_SHOWN);
	}

	// Enable/Disable window resizable mode
	if (borderlessEnabled)
	{
		SDL_SetWindowBordered(App->window->window, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowBordered(App->window->window, SDL_TRUE);
	}

	// Enable/Disable window resizable mode
	if (resizableEnabled)
	{
		SDL_SetWindowResizable(App->window->window, SDL_TRUE);
	}
	else
	{
		SDL_SetWindowResizable(App->window->window, SDL_FALSE);
	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("EDITOR: Destroying SDL window and quitting all SDL systems!", NULL);

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}