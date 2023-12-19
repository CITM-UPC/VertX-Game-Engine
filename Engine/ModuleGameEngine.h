#pragma once
#include <list>
#include "Globals_ENGINE.h"
#include "Engine_Module.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "Camera.h"
#include "EngineScene.h"

class ModuleGameEngine
{
public:
	
	ModuleRenderer3D_ENGINE* renderer3D_engine;
	Engine_ModuleScene* scene;

	GameObject cameraGO;

private:

	std::list<Engine_Module*> list_modules;

public:

	ModuleGameEngine();
	~ModuleGameEngine();

	bool Init();
	bool Start();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

