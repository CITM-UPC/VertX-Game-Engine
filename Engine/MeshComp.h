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
	void EncloseAABBWithOBB(AABB& aabb, const OBB& obb);
	void TransformOBB(OBB& obb, const glm::mat4& transformationMatrix);
	void SetOBBFromAABB(OBB& obb, const AABB& aabb);
	~Meshes();

	void Update();
	void Inspector();

	MyMesh* mesh;
};
