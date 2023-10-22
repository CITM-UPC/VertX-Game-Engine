#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL2/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SetTitle(const char* title);

public:

	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


	bool fullscreenEnabled = WIN_FULLSCREEN;
	bool borderlessEnabled = WIN_BORDERLESS;
	bool resizableEnabled = WIN_RESIZABLE;
	bool fullcreenDesktopEnabled = WIN_FULLSCREEN_DESKTOP;

	float windowBrightness = 1.0f;
};

#endif // __ModuleWindow_H__