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

void GameObject::removeChild(GameObject* child)
{
	auto it = std::remove_if(childs.begin(), childs.end(), [child](const std::unique_ptr<GameObject>& ptr) {
		return ptr.get() == child;
		});

	// Check if the child was found
	if (it != childs.end())
	{
		// Erase the element at the end, which was moved there by std::remove_if
		childs.erase(it, childs.end());
		// The unique_ptr will automatically delete the removed child
	}
}

void GameObject::Move(GameObject* newParent)
{
	auto it = std::find_if(parent->childs.begin(), parent->childs.end(), [this](const std::unique_ptr<GameObject>& child) {
		return child.get() == this;
		});

	if (it != parent->childs.end()) {
		// Move the child to the new list
		newParent->childs.push_back(std::move(*it));
		parent->childs.erase(it);

		// Update the parent pointer of the moved child
		parent = newParent;
	}
}