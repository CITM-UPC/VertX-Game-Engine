#pragma once
#include<vector>
#include<string>

#include "Component.h"
#include "imgui.h"

enum class ComponentType;
class Transform;
class Meshes;
class CObject;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void CreateInspector();

	std::string name;
	GameObject* Parent;
	std::vector<GameObject*> child;
	std::vector<Component*> Comp;

	Transform* transform = nullptr;
	Meshes* GOMesh = nullptr;
	CObject* CamGOGame;
	char Title[150] = "Write Text";

};
