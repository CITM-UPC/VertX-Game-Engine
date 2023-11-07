#include "ModuleGameEngine.h"
#include "..\Editor\Globals.h"

ModuleGameEngine::ModuleGameEngine()
{
	//input = new ModuleInput(this);
	renderer3D_engine = new ModuleRenderer3D_ENGINE(this);

	//AddModule(input);
	AddModule(renderer3D_engine);
}

ModuleGameEngine::~ModuleGameEngine()
{
	list_modules.clear();
}

bool ModuleGameEngine::Init()
{
	bool ret = true;

	for (auto const& item : list_modules)
	{
		ret = item->Init();
	}

	return ret;
}

bool ModuleGameEngine::Start()
{
	bool ret = true;
	
	LOG("ENGINE: Game Engine Start --------------", NULL);
	
	for (auto const& item : list_modules)
	{
		ret = item->Start();
	}

	return ret;
}

void ModuleGameEngine::PrepareUpdate()
{
}

void ModuleGameEngine::FinishUpdate()
{
}

engine_update_status ModuleGameEngine::PreUpdate()
{
	engine_update_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	return ret;
}

engine_update_status ModuleGameEngine::Update()
{
	engine_update_status ret = ENGINE_UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();
	return ret;
}

engine_update_status ModuleGameEngine::PostUpdate()
{
	engine_update_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

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