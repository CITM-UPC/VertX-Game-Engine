#pragma once
#include "Engine_Module.h"
#include "Globals_ENGINE.h"
#include "SDL2/SDL.h"
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>

struct Folder {
	std::string name;
	std::vector<Folder> subfolders;
	std::map<std::string, std::vector<std::string>> files;
};

class ModuleRenderer3D_ENGINE : public Engine_Module
{
public:
	ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled = true);
	~ModuleRenderer3D_ENGINE();

	bool Init();
	engine_update_status PreUpdate();
	engine_update_status Update();
	engine_update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	void DrawGrid(int size, int step, bool xzAxis = true);

	void DrawAxis(float lineWidth = 1.0f);

	void SetTargetWindow(SDL_Window* target) { 
		targetWindow = target; 
	}
	void SetScreenSize(int width, int height) { 
		screen_width = width; 
		screen_height = height;
	}
	void SetVsync(bool active) { 
		vsync = active; 
	}

	void HandleFileDrop(const std::string& filePath, Folder& rootFolder);
	bool RecursiveRemoveDirectory(const char* directory);
	bool CleanUpAssets();
	void CreateDirectoryIfNotExists(const char* directory);
	void AddDynamicFolder(Folder& parentFolder, const std::string& folderName);
	void RenderFoldersAndFiles(const Folder& folder);

	

public:

	SDL_GLContext context;

	mat3f normalMatrix;
	mat4f modelMatrix;
	mat4f viewMatrix;
	mat4f projectionMatrix;

	const char* parentDirectory = "Assets";
	const char* fbxAssetsDirectory = "Assets\\FBX_Assets";
	const char* imageAssetsDirectory = "Assets\\Image_Assets";

private:

	SDL_Window* targetWindow;

	
	
	bool vsync;
	int screen_width;
	int screen_height;

};