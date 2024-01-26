#include "Globals_ENGINE.h"
#include "ModuleGameEngine.h"
#include "EngineScene.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../Editor/Application.h"


namespace fs = std::filesystem;

Engine_ModuleScene::Engine_ModuleScene(ModuleGameEngine* gEngine, bool start_enabled) : Engine_Module(gEngine, start_enabled)
{
}

// Destructor
Engine_ModuleScene::~Engine_ModuleScene() = default;

bool Engine_ModuleScene::Init()
{
	fs::create_directories("VertX/Library/Meshes/");
	fs::create_directories("VertX/Library/Materials/");
	fs::create_directories("VertX/Library/Scripts/");

	addGameObject("VertX/Assets/Street/StreetEnvironment.fbx");
	addGameObject("VertX/Assets/BakerHouse.fbx");
	//addGameObject("VertX/Assets/waterfountain.fbx");
	

	return true;
}

engine_status Engine_ModuleScene::PreUpdate() { return ENGINE_UPDATE_CONTINUE; }

engine_status Engine_ModuleScene::Update()
{
	if (!paused)
	{
		for (auto& GO : currentScene.gameObjectList)
		{
			recursiveObjectUpdate(GO.get());
		}

		if (step)
		{
			step = false;
			paused = true;
		}
	}

	if (step && paused) paused = false;

	return ENGINE_UPDATE_CONTINUE;
}

engine_status Engine_ModuleScene::PostUpdate()
{
	for (auto& GO : currentScene.gameObjectList)
	{
		glPushMatrix();
		glMultMatrixd(&GO->GetComponent<Transform>()->_transformationMatrix[0].x);

		recursiveObjectRendering(GO.get());

		glPopMatrix();

		if (bboxDebug) GO->drawAABBox(GO->computeAABB());
	}

	return ENGINE_UPDATE_CONTINUE;
}

bool Engine_ModuleScene::CleanUp() { return true; }

void Engine_ModuleScene::recursiveObjectUpdate(GameObject * GoToUpdate)
{
	GoToUpdate->UpdateComponents();

	if (!GoToUpdate->childs.empty())
	{
		for (auto& child : GoToUpdate->childs)
		{
			recursiveObjectUpdate(child.get());
		}
	}
}

void Engine_ModuleScene::recursiveObjectRendering(GameObject * GoToRender)
{
	GoToRender->RenderComponents();

	if (!GoToRender->childs.empty())
	{
		for (auto& child : GoToRender->childs)
		{
			recursiveObjectRendering(child.get());
		}
	}
}

void Engine_ModuleScene::addGameObject()
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = "GameObject";
	int currentCopies = checkNameAvailability(meshName);
	if (currentCopies > 0) {
		meshName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = meshName;

	gameObjectToAdd->UUID = game_engine->generateUUID32();

	currentScene.gameObjectList.push_back(std::move(gameObjectToAdd));

	/*gEngine->logHistory.push_back("[Engine] Add GameObject");*/
}

void Engine_ModuleScene::addGameObject(const std::string & filePath)
{
	/*gEngine->logHistory.push_back("[Engine] Add GameObject with path " + filePath);*/

	addGameObject();

	auto& gOparent = currentScene.gameObjectList.back();

	auto meshInfo_vector = MeshLoader::loadMeshFromFile(filePath);
	auto texture_paths_vector = MeshLoader::loadTextureFromFile(filePath);

	int i = 0;
	for (auto& meshInfo : meshInfo_vector)
	{
		std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

		std::string fileName = filePath;
		eraseBeforeDelimiter(fileName);
		std::string meshName = fileName;
		deleteSubstring(meshName, ".fbx");

		int currentCopies = checkNameAvailability(meshName, gOparent.get());
		if (currentCopies > 0) {
			meshName.append("(" + std::to_string(currentCopies) + ")");
		}
		gameObjectToAdd->name = meshName;
		gameObjectToAdd->UUID = game_engine->generateUUID32();

		gameObjectToAdd->parent = gOparent.get();

		gOparent->childs.push_back(std::move(gameObjectToAdd));

		string folderName = "VertX/Library/Meshes/";

		ofstream oFile(folderName + meshName + ".mesh", ios::binary);
		oFile << meshInfo;
		oFile.close();

		/*gEngine->logHistory.push_back("[Engine] Mesh file created as " + meshName + ".mesh in " + folderName);*/

		gOparent->childs.back().get()->GetComponent<Transform>()->_transformationMatrix = meshInfo._transformationMatrix;

		Texture2D textureToPush(gOparent->childs.back().get(), texture_paths_vector.at(i));
		gOparent->childs.back().get()->AddComponent<Texture2D>(textureToPush);

		Mesh meshToPush(gOparent->childs.back().get(), meshInfo, Mesh::Formats::F_V3T2);
		gOparent->childs.back().get()->AddComponent<Mesh>(meshToPush);

		gOparent->childs.back().get()->GetComponent<Mesh>()->setName(fileName);
		gOparent->childs.back().get()->GetComponent<Mesh>()->texture = gOparent->childs.back().get()->GetComponent<Texture2D>();
		i++;

		/*gEngine->logHistory.push_back("[Engine] Mesh loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
			+ std::to_string(meshInfo._numIndexs) + " indexs, "
			+ std::to_string(meshInfo._numNormals) + " normals, "
			+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
			+ std::to_string(meshInfo._numVerts) + " vertexs.");*/
	}
}

void Engine_ModuleScene::addGameObject(Primitive * shape)
{
	std::unique_ptr<GameObject> gameObjectToAdd = std::make_unique<GameObject>();

	std::string meshName = shape->GetType();

	std::string goName = meshName;
	int currentCopies = checkNameAvailability(goName);
	if (currentCopies > 0) {
		goName.append("(" + std::to_string(currentCopies) + ")");
	}
	gameObjectToAdd->name = goName;
	currentScene.gameObjectList.push_back(std::move(gameObjectToAdd));

	MeshInfo meshInfo(shape->getVertexData()->data(), shape->getVertexData()->size(), shape->getIndexData()->data(), shape->getIndexData()->size(), shape->GetNumTexCoords(), shape->GetNumNormals(), shape->GetNumFaces());
	Mesh meshToPush(currentScene.gameObjectList.back().get(), meshInfo, Mesh::Formats::F_V3);

	currentScene.gameObjectList.back().get()->AddComponent<Mesh>(meshToPush);
	currentScene.gameObjectList.back().get()->GetComponent<Mesh>()->setName(meshName);

	/*gEngine->logHistory.push_back("[Engine] Mesh (" + meshName + ") loaded with " + std::to_string(meshInfo._numFaces) + " faces, "
		+ std::to_string(meshInfo._numIndexs) + " indexs, "
		+ std::to_string(meshInfo._numNormals) + " normals, "
		+ std::to_string(meshInfo._numTexCoords) + " tex coords, and "
		+ std::to_string(meshInfo._numVerts) + " vertexs.");*/
}

void Engine_ModuleScene::removeGameObject(GameObject * GOtoDelete)
{
	auto it = std::find_if(currentScene.gameObjectList.begin(), currentScene.gameObjectList.end(), [GOtoDelete](const std::unique_ptr<GameObject>& GO) {
		return GO.get() == GOtoDelete;
		});

	if (it != currentScene.gameObjectList.end())
	{
		currentScene.gameObjectList.erase(it);
	}
}

void Engine_ModuleScene::NewScene()
{
	currentScene.fileName = "";
	currentScene.name = "";
	currentScene.gameObjectList.clear();
}

void Engine_ModuleScene::LoadRootGameObject(string name, unsigned long UUID, bool active)
{
	std::unique_ptr<GameObject> newRootGO = std::make_unique<GameObject>();

	currentScene.gameObjectList.push_back(std::move(newRootGO));

	currentScene.gameObjectList.back()->name = name;
	currentScene.gameObjectList.back()->UUID = UUID;
	currentScene.gameObjectList.back()->isActive = active;
}

GameObject* Engine_ModuleScene::findGameObjectfromUUID(GameObject * head, unsigned long UUIDtocompare)
{
	for (auto& child : head->childs)
	{
		if (child.get()->UUID == UUIDtocompare)
		{
			return child.get();
		}

		GameObject* tempParent = findGameObjectfromUUID(child.get(), UUIDtocompare);
		if (tempParent != nullptr)
		{
			return tempParent;
		}
	}

	return nullptr;
}

void Engine_ModuleScene::LoadComponentMesh(GameObject * owner, string path)
{
	MeshInfo newinfo = MeshInfo();

	std::ifstream meshfile(path, ios::binary);

	if (meshfile.is_open())
	{
		meshfile >> newinfo;

		Mesh newMesh(owner, newinfo, Mesh::Formats::F_V3T2);

		owner->AddComponent<Mesh>(newMesh);
	}
	else
		/*gEngine->logHistory.push_back("Mesh Binary File could not be open");*/

	meshfile.close();
}

void Engine_ModuleScene::LoadComponentTexture(GameObject * owner, string path)
{
	size_t lastOf = path.find_last_of('/');
	std::string fileName = path.substr(lastOf + 1);
	Texture2D textureToPush(owner, fileName);
	owner->AddComponent<Texture2D>(textureToPush);
}