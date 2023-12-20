#include "Globals_ENGINE.h"
#include "ModuleGameEngine.h"
#include "EngineScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

Engine_ModuleScene::Engine_ModuleScene(ModuleGameEngine* game_engine, bool start_enabled) : Engine_Module(game_engine, start_enabled)
{
}

// Destructor
Engine_ModuleScene::~Engine_ModuleScene() = default;

bool Engine_ModuleScene::Init()
{
	fs::create_directories("Assets/Library/");

	addGameObject("Assets/Street_Environment.fbx");

	return true;
}

engine_status Engine_ModuleScene::PreUpdate() { return ENGINE_UPDATE_CONTINUE; }

engine_status Engine_ModuleScene::Update() { return ENGINE_UPDATE_CONTINUE; }

engine_status Engine_ModuleScene::PostUpdate()
{
	for (auto& vector : gameObjectList)
	{
		for (auto& childs : vector.get()->childs)
		{
			childs->UpdateComponents();
		}
	}

	return ENGINE_UPDATE_CONTINUE;
}

bool Engine_ModuleScene::CleanUp() { return true; }

void Engine_ModuleScene::addEmptyGameObject()
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = "GameObject";
	int currentCopies = checkNameAvailability(meshName);
	if (currentCopies > 0) {
		meshName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = meshName;

	gameObjectList.push_back(std::move(gameObjectToAdd));
}

void Engine_ModuleScene::addGameObject(const std::string & filePath)
{

	addEmptyGameObject();

	auto& gOparent = gameObjectList.back();

	auto meshInfo_vector = MeshLoader::loadMeshFromFile(filePath);
	auto texture_paths_vector = MeshLoader::loadTextureFromFile(filePath);

	/*std::string parentName;*/

	int i = 0;
	for (auto& meshInfo : meshInfo_vector)
	{
		std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

		std::string fileName = filePath;
		eraseBeforeDelimiter(fileName);
		std::string meshName = fileName;
		deleteSubstring(meshName, ".fbx");

		/*parentName = meshName;*/

		int currentCopies = checkNameAvailability(meshName, gOparent.get());
		if (currentCopies > 0) {
			meshName.append("(" + std::to_string(currentCopies) + ")");
		}
		gameObjectToAdd->name = meshName;

		gOparent->childs.push_back(std::move(gameObjectToAdd));

		string folderName = "Assets/Library/";

		ofstream oFile(folderName + meshName + ".fbx", ios::binary);
		oFile << meshInfo;
		oFile.close();

		Texture2D textureToPush(gOparent->childs.back().get(), texture_paths_vector.at(i));
		gOparent->childs.back().get()->AddComponent<Texture2D>(textureToPush);

		Mesh meshToPush(gOparent->childs.back().get(), meshInfo, Mesh::Formats::F_V3T2);
		gOparent->childs.back().get()->AddComponent<Mesh>(meshToPush);

		gOparent->childs.back().get()->GetComponent<Mesh>()->setName(fileName);
		gOparent->childs.back().get()->GetComponent<Mesh>()->texture = gOparent->childs.back().get()->GetComponent<Texture2D>();
		i++;
	}
}

void Engine_ModuleScene::SaveScene() {}
void Engine_ModuleScene::LoadScene() {}