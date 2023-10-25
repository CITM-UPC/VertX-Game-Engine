#pragma once
#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include "imgui.h"

class GameObject;
class Component;
class aiNode;

class Transform : public Component
{
public:
	Transform();
	Transform(GameObject* Transform);
	~Transform();

	void Inspector();
	void Tranformation();

	glm::mat4x4 Transform_Matrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	glm::vec3 position = { 0,0,0 };
	float positionArray[3] = { position.x, position.y, position.z };

	glm::vec3 scale = { 1,1,1 };
	float scaleArray[3] = { scale.x, scale.y, scale.z };

	glm::vec3 rotate = { 0,0,0 };
	float rotationArray[3] = { rotate.x, rotate.y, rotate.z };

	float angle = 180;

private:

};