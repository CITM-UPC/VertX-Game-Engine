#pragma once

class GameObject;

enum class Type
{
	None,
	Transform,
	MeshRenderer,
	CamObject,
};

class Component
{
public:

	Component();
	Component(GameObject* Object);
	~Component();
	virtual void Update();

	virtual void GameCameraMovement(GameObject* CamObject);

	virtual void Inspector();
	bool active;
	Type type;
	GameObject* GObjectSelected = nullptr;

};