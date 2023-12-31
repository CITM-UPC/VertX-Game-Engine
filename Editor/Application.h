#pragma once

#include <list>
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"
#include "ModuleImGUI.h"
#include "..\Engine\ModuleGameEngine.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer* renderer;
	ModuleImGUI* imgui;
	ModuleGameEngine* game_engine;

private:

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	
	void OpenWebLink(const char *webLink);

	int fps = FPS;
	double deltaTime;
	double frameDuration;

	/*bool isSimulationRunning = false;
	bool isSimulationPaused = false;
	std::chrono::high_resolution_clock::time_point simulationTime;
	std::chrono::high_resolution_clock::time_point lastSimulationTime;*/

	// Timer variables
	bool isPlaying = false;
	bool isPaused = false;
	double startTime = 0.0;
	double elapsedTime = 0.0;


private:

	std::vector<std::string> consoleLogs;

public:

	void AddConsoleLog(char* log);
	void ClearConsoleLogs();

	std::vector<std::string> GetConsoleLogs();

};

extern Application* App;