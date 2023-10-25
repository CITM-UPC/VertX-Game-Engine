#pragma once
#include "Module.h"
#include "Globals.h"
#include "Component.h"
#include <glm/glm.hpp>
#include "GameObject.h"
#include <string>

struct OBB {
	glm::mat4 transform;
	glm::vec3 halfExtents;
};

struct AABB {
	glm::vec3 minCorner;
	glm::vec3 maxCorner;
};

using namespace std;

class GameObject;
class Transform;

struct MyMesh {
	MyMesh();
	~MyMesh();

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
	GameObject* OBmesh = nullptr;

	AABB Gbbox;
	AABB Laabb;
	OBB obb;
	bool IsVisible = false;
	void Render();
};

class ModuleLoadFBX : public Module
{
public:

	ModuleLoadFBX(Application* app, bool start_enabled = true);

	bool Start();
	GameObject* LoadFile(string file_path, string nameGO);
	GameObject* PrimitivesObjects(int Case);
	void LoadMesh(MyMesh* mesh);

	update_status PostUpdate(float dt);
	//bool Init();
	bool CleanUp();
	GameObject* MeshObject = nullptr;
private:
	vector<MyMesh*> meshes;

};