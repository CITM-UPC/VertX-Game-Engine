#pragma once
#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"

class GameObject;
class Component;
struct MyMesh;

class Meshes : public Component
{
public:
	Meshes();
	Meshes(GameObject* Meshobject);
	~Meshes();

	void Update();
	void Inspector();

	MyMesh* mesh;
};
