#pragma once

#include <list>
#include "Globals_ENGINE.h"
#include "Engine_Module.h"

//#include "ModuleInput.h"
#include "ModuleRenderer3D_ENGINE.h"
#include "EngineScene.h"
//#include "ModuleAudio.h"

#include <IL/il.h>
#include <random>
#include "Camera.h"
#include "pcg_random.hpp"

using UUID32 = uint32_t;

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

	// Function to generate a random 32-bit UUID using PCG
	UUID32 generateUUID32() {
		pcg32 rng(std::random_device{}());

		// Generate a random 32-bit UUID
		return rng();
	}

	int volume;

	double volumeLevel;

	glm::vec3 cameraPosition;

private:

	void AddModule(Engine_Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

