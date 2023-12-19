#pragma once
#include "Engine_Module.h"
#include "Globals_ENGINE.h"
#include "SDL2/SDL.h"
#include <glm/gtc/type_ptr.hpp>

#include "GL/glew.h"
#include <map>
#include "SDL2/SDL_opengl.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "Mesh.h"
#include <string>
#include "MeshLoad.h"
#include "GameObject.h"

class ModuleRenderer3D_ENGINE : public Engine_Module
{
public:
	ModuleRenderer3D_ENGINE(ModuleGameEngine* game_engine, bool start_enabled = true);
	~ModuleRenderer3D_ENGINE();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
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


	void deleteSubstring(std::string& mainString, const std::string& substringToDelete) {
		size_t pos = mainString.find(substringToDelete);

		// Iterate until all occurrences are removed
		while (pos != std::string::npos) {
			mainString.erase(pos, substringToDelete.length());
			pos = mainString.find(substringToDelete, pos);
		}
	}

	void eraseBeforeDelimiter(std::string& str) {
		size_t found = str.find_last_of("\\/");
		if (found != std::string::npos) {
			str.erase(0, found + 1);
		}
	}

	void detectAndIncrement(std::string mainString, const std::string& substring, int& counter) {
		if (mainString.find(substring) != std::string::npos) {
			counter++;
		}
	}

	int checkNameAvailability(std::string name) {
		int count = 0;

		for (const auto& vector : gameObjectList) {
			detectAndIncrement(vector.name, name, count);
		}

		return count;
	}

	void HandleFileDrop(const char* filePath);
	bool RecursiveRemoveDirectory(const char* directory);
	bool CleanUpAssets();
	void CreateDirectoryIfNotExists(const char* directory);

	

public:

	SDL_GLContext context;

	glm::mat3x3 normalMatrix;
	glm::mat4x4 modelMatrix;
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;
	std::list<GameObject> gameObjectList;

	const char* parentDirectory = "Assets";
	const char* fbxAssetsDirectory = "Assets\\Library\\Meshes";
	const char* imageAssetsDirectory = "Assets\\Library\\Textures";

	bool vsync;

	int screen_width;
	int screen_height;

	bool glDepthTestIsEnabled = true;
	bool glCullFaceIsEnabled = true;
	bool glColorMaterialIsEnabled = true;
	bool glLightingIsEnabled = false;

private:

	SDL_Window* targetWindow;

};