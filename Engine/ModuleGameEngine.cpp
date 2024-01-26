#include "ModuleGameEngine.h"
#include "..\Editor\Globals.h"

ModuleGameEngine::ModuleGameEngine()
{
	//input = new ModuleInput(this);
	renderer3D_engine = new ModuleRenderer3D_ENGINE(this);
	scene = new Engine_ModuleScene(this);

	//AddModule(input);
	AddModule(renderer3D_engine);
	AddModule(scene);

	cameraGO.AddComponent(Component::Type::CAMERA);
}

ModuleGameEngine::~ModuleGameEngine()
{
	list_modules.clear();
}

bool ModuleGameEngine::Init()
{
	bool ret = true;

	//for (auto const& item : list_modules)
	//{
	//	ret = item->Init();
	//}

	return ret;
}

bool ModuleGameEngine::Start()
{
	bool ret = true;
	
	LOG("ENGINE: Game Engine Start --------------", NULL);
	
	//for (auto const& item : list_modules)
	//{
	//	ret = item->Start();
	//}

	return ret;
}

void ModuleGameEngine::PrepareUpdate()
{
}

void ModuleGameEngine::FinishUpdate()
{
}

engine_status ModuleGameEngine::PreUpdate()
{
	const glm::mat4& cameraGlobalTransform = cameraGO.getGlobalTransform();

	// Extract the translation components from the global transform
	glm::vec3 cameraPosition = glm::vec3(cameraGlobalTransform[3]);

	// Calculate Euclidean distance from (0, 0, 0)
	double distanceFromOrigin = glm::length(cameraPosition);

	// Set a threshold distance, beyond which the sound is not heard
	double thresholdDistance = 25;

	// Map the distance to a volume level (adjust the mapping as needed)
	volumeLevel = 1.0 - std::min(distanceFromOrigin / thresholdDistance, 1.0);

	// Adjust the volume level to the desired range for SDL_mixer (0-128)
	volume = static_cast<int>(volumeLevel * 128.0);

	// Log debug information
	LOG("Camera Position: (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	LOG("Distance from Origin: %f", distanceFromOrigin);
	LOG("Volume Level: %f", volumeLevel);
	LOG("Mapped Volume: %d", volume);

	engine_status ret = ENGINE_UPDATE_CONTINUE;

	return ret;
}

engine_status ModuleGameEngine::Update()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;
	return ret;
}

engine_status ModuleGameEngine::PostUpdate()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;

	return ret;
}

bool ModuleGameEngine::CleanUp()
{
	bool ret = true;

	LOG("ENGINE: Game Engine CleanUp --------------", NULL);

	for (auto const& item : list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	return ret;
}

void ModuleGameEngine::AddModule(Engine_Module* mod)
{
	list_modules.push_back(mod);
}