#pragma once
#include <memory>

class GameObject;

class Component
{
public:

	enum class Type
	{
		TRANSFORM,
		MESH,
		TEXTURE
	};

	GameObject* gameObject;
	bool isActive = true;
	

public:

	virtual void Update() = 0;
	virtual Type getType() const = 0;
};
