#pragma once

#include "Engine_Module.h"
#include "Globals_ENGINE.h"

class Engine_ModuleScene : public Engine_Module
{
public:
	Engine_ModuleScene(GameEngine* gEngine, bool start_enabled = true);
	~Engine_ModuleScene();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

	void SaveScene();
	void LoadScene();

public:
	

private:
};