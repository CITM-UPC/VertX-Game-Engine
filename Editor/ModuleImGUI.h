#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleImGUI : public Module
{
public:
	ModuleImGUI(Application* app, bool start_enabled = true);
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	void RenderImGUI();

private:
	bool dockSpaceEnabled = true;
	bool options = false;
	bool about = false;
	bool demoWindow = false;
};