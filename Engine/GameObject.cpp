#include "GameObject.h"
#include <memory>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <Windows.h>
#include <string>
#include "Tree.hpp"
#include "Camera.h"

GameObject::GameObject()
{
	name = "";
	Transform transformToPush(this);
	AddComponent(transformToPush);
}

GameObject::~GameObject()
{
	components.clear();
};

std::list<std::unique_ptr<Component>>* GameObject::GetComponents()
{
	return &components;
}

void GameObject::AddComponent(Component::Type component)
{
	switch (component)
	{
	case Component::Type::TRANSFORM:
		components.emplace_back(std::make_unique<Transform>(this));
		break;
	case Component::Type::MESH:
		components.emplace_back(std::make_unique<Mesh>(this));
		break;
	case Component::Type::TEXTURE2D:
		components.emplace_back(std::make_unique<Texture2D>(this));
		break;
	case Component::Type::CAMERA:
		components.emplace_back(std::make_unique<Camera>(this));
		break;
	default:
		break;
	}
}

void GameObject::RemoveComponent(Component::Type component)
{
	for (auto& comp : components)
	{
		if (comp->getType() == component)
		{
			components.erase(std::remove(components.begin(), components.end(), comp));
			break;
		}
	}
}

void GameObject::UpdateComponents()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}