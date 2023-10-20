#pragma once

class ModuleGameEngine;

class Engine_Module
{
private :
	bool enabled;

public:
	ModuleGameEngine* game_engine;

	Engine_Module(ModuleGameEngine* parent, bool start_enabled = true) : game_engine(parent)
	{}

	virtual ~Engine_Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual engine_update_status PreUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_update_status Update()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_update_status PostUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
};