#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>
#include <cstring>
#include "..\Engine\GameObject.h"
#include "..\Engine\Component.h"
#include "TextEditor.h"
#include <filesystem>

namespace fs = std::filesystem;

struct InfrastructureInfo {
	std::string sdl_version_compiled;
	std::string sdl_version_linked;
	std::string gl_version;
	std::string devil_version;

	std::string GpuVendor;
	std::string Gpu;
	std::string GpuDriver;

	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;

	uint cpu_count = 0;
	uint l1_cachekb = 0;
};

class ModuleImGUI : public Module
{
public:
	ModuleImGUI(Application* app, bool start_enabled = true);
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	void RenderUI();

	vec3 GetSelectedObjectPos();
	void SetSelectedObjectTexture(std::string filePath);

	Uint32 musicCycleStart = 0;
	bool musicplaying = false;
	bool musicPlayedThisCycle = false;
	bool alternateTracks = false;

	bool ingame = false;

private:

	update_status MainMenuBar();

	void FPSGraphWindow();
	void HierarchyWindow();
	void InspectorWindow();
	void LogConsoleWindow();
	void OptionsWindow();
	void CamDebugWindow();
	void AboutWindow();
	void FileExplorerWindow();
	void HierarchyRecursive(GameObject* gO);
	void GameObjectOptions();
	void ReparentMenu();
	void SaveAsMenu();
	void LoadSceneMenu();
	void GetInfrastructureInfo();
	void ShowFolderContents(const fs::path& folderPath);
	void RenderImGUIAssetsWindow();
	void MoveGOBackAndForward(int meters);
	void InGameController();
	void ToggleWindow();

	bool controlNodeOpen = true;

private:

	const std::string assetFolderPath = "VertX/Assets";

	bool dockSpaceEnabled = true;

	bool hierarchy = true;
	bool inspector = true;
	bool logWindow = false;
	bool assetsWindow = true;
	bool FPSgraph = false;
	bool options = false;
	bool camDebug = false;
	bool about = false;
	bool demo = false;
	bool reparentMenu = false;
	bool saveasMenu = false;
	bool loadMenu = false;
	bool fileExplorer = false;
	bool controller = false;
	bool toggle = false;

	char Title[150] = "Write Text";
	std::string name;

	Uint32 engineFX;

	bool fx = true;

	bool soundeffectplayed = false;

	bool effectMusicPlayed = false;

	bool effectMusicPlayed2 = false;

	bool snapshot = false;

	bool autoScrollLog = true;

	double distanceFromOrigin;

	double movingdistanceFromOrigin;

	int volumemove;


	Uint32 set = 0;

	int ticks = 0;
	bool switchsong = false;

	double volumeLevel;
	bool fxplaying = false;
	bool fxplaying2 = false;

	bool reparentThis = true;
	bool reparentTo = false;

	glm::vec3 cameraPosition;

	int volume;

	int movingTicks = 0;
	bool increasing = true;

	bool canMoveGO = true;

	std::string aboutContent;
	std::string filePath;
	std::string fileContent;

	// Hardware information
	InfrastructureInfo info;
	GameObject* gameObjSelected = nullptr;

	// Game Object which parent will be changed
	GameObject* orphan = nullptr;

	// Objective Parent that will adopt the child
	GameObject* adopter = nullptr;

	TextEditor editor;
};