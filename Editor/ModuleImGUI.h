#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include <IL/il.h>
#include <vector>
#include <filesystem>
#include "../Engine/GameObject.h"
#include "../Engine/Component.h"
#include <map>    
#include <unordered_map>
#include <IL/il.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


namespace fs = std::filesystem;


class ModuleImGUI : public Module
{
public:
	ModuleImGUI(Application* app, bool start_enabled = true);
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	std::vector<std::string> assetFiles;

public:

	void RenderImGUI();

	

	bool showContextMenu = false;

	void RenderImGUIAssetsWindow();
	void RenderImGUIAboutWindow();
	void RenderImGUIConfigWindow();
	void RenderImGUIHierarchyWindow();
	void RenderImGUIInspectorWindow();
	void RenderImGUICameraInspectorWindow();
	void RenderImGUIDebugLogWindow();
	void RenderImGUIConsoleWindow();

	void RenderFPSGraph();

	// Function to display a Tool Tip message for any mouse hovered item. Call it AFTER the item creation.
	void ToolTipMessage(const char* tip);

	void GeneratePrimitives();


	bool CreatedOnce = true;

	std::string nameholder;
	bool renamed = false;

	bool g_IsDragging = false; // Boolean flag to indicate whether an asset is being dragged.
	std::string g_DraggedAssetName;


	GameObject gameObjSelected;

private:
	
	bool aboutWindow = false;
	bool configWindow = false;
	bool assetsWindow = true;
	bool cameraInspectorWindow = true;
	bool consoleWindow = true;
	bool hierarchyWindow = true;
	bool inspectorWindow = true;
	bool showImGuiDebugLogWindow = false;

	bool TexEnabled = true;

	int numVerts;
	int numIndexes;
	std::string Name; 
	std::string MeshFileName;
	std::string MeshTexture;
	

	char Title[150] = "Write Text";
	std::string name;

	const std::string assetFolderPath = "Assets";

	std::vector<std::string> GetAssetFiles(const std::string& assetFolderPath) {
		std::vector<std::string> assetFiles;
		for (const auto& entry : fs::directory_iterator(assetFolderPath)) {
			if (fs::is_regular_file(entry.path())) {
				assetFiles.push_back(entry.path().filename().string()); // Convert path to string
			}
		}
		return assetFiles;
	}

};