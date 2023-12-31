#pragma once

#include "Engine_Module.h"
#include "Globals_ENGINE.h"

class Engine_ModuleScene : public Engine_Module
{
public:
	Engine_ModuleScene(ModuleGameEngine* game_engine, bool start_enabled = true);
	~Engine_ModuleScene();

	bool Init();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

	void SaveScene();
	void LoadScene();

public:
	void addEmptyGameObject();

	void addGameObject(const std::string& filePath);

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

	int checkNameAvailability(std::string name, GameObject* parent) {
		int count = 0;

		for (const auto& vector : parent->childs) {
			detectAndIncrement(vector->name, name, count);
		}

		return count;
	}
	int checkNameAvailability(std::string name) {
		int count = 0;

		for (const auto& vector : gameObjectList) {
			detectAndIncrement(vector->name, name, count);
		}

		return count;
	}

public:
	std::list<std::unique_ptr<GameObject>> gameObjectList;

private:
};