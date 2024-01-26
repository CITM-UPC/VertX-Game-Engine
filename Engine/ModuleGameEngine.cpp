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