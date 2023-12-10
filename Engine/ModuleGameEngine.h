#pragma once
#include <list>
#include "Globals_ENGINE.h"
#include "Engine_Module.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "Camera.h"

class ModuleGameEngine
{
public:
	
	ModuleRenderer3D_ENGINE* renderer3D_engine;

	Camera camera;

	GameObject scene;

private:

	std::list<Engine_Module*> list_modules;

public:

	ModuleGameEngine();
	~ModuleGameEngine();

	bool Init();
	bool Start();
	engine_update_status PreUpdate();
	engine_update_status Update();
	engine_update_status PostUpdate();
	bool CleanUp();

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

