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

public:

	void RenderImGUI();

	void RenderImGUIAboutWindow();

private:
	
	
	bool aboutWindow = false;
	bool optionsWindow = false;
};