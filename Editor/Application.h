#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <list>
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"
#include "ModuleImGUI.h"
#include "ModuleResources.h"
#include "..\Engine\ModuleGameEngine.h"
#include "ModuleAudio.h"

using namespace std;
using namespace chrono;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer* renderer;
	ModuleResources* resources;
	ModuleImGUI* imgui;
	ModuleAudio* audio;
	ModuleGameEngine* game_engine;
	
	int targetFPS;
	duration<double> frameDurationTime;

private:

	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	vector<float> fpsHistory;
	list<string> logHistory;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	
	void OpenWebLink(const char *webLink);

private:

	std::vector<std::string> consoleLogs;

public:

	void AddConsoleLog(char* log);
	void ClearConsoleLogs();

	std::vector<std::string> GetConsoleLogs();

};

extern Application* App;