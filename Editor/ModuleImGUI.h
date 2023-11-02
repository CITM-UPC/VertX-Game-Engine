#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"


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

	void RenderImGUIAssetsWindow();
	void RenderImGUIAboutWindow();
	void RenderImGUIConfigWindow();
	void RenderImGUIHierarchyWindow();
	void RenderImGUIInspectorWindow();
	void RenderImGUICameraInspectorWindow();
	void RenderImGUIDebugLogWindow();

	void RenderFPSGraph();

	// Function to display a Tool Tip message for any mouse hovered item. Call it AFTER the item creation.
	void ToolTipMessage(const char* tip);

	void GeneratePrimitives();
	void Hierarchy(GameObject* parent);

	bool hierarchy = true;

	GameObject* RootGO = nullptr;
	GameObject* Selected = nullptr;
	bool CreatedOnce = true;

private:
	
	bool aboutWindow = false;
	bool configWindow = false;

	bool assetsWindow = true;
	bool cameraInspectorWindow = true;
	bool hierarchyWindow = true;
	bool inspectorWindow = true;
	bool showDebugLogWindow = true;

};