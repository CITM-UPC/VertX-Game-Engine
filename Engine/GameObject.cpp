#include "GameObject.h"
#include <memory>

GameObject::GameObject()
{
	name = "";
	components.push_back(std::make_shared<Transform>(*this));
}

GameObject::~GameObject() = default;

std::vector<std::shared_ptr<Component>> GameObject::GetComponents()
{
	//Initialize in Template so Flexible Usage
	return components;
}

void GameObject::AddComponent(Component::Type component)
{
	std::shared_ptr<Component> ptr;

	//Point to Game Object features:
	switch (component)
	{
	case Component::Type::TRANSFORM:
		ptr = std::make_shared<Transform>(*this);
		break;
	case Component::Type::MESH:
		ptr = std::make_shared<Mesh>(*this);
		break;
	case Component::Type::TEXTURE:
		ptr = std::make_shared<Texture2D>(*this);
		break;
	default:
		break;
	}

	components.push_back(ptr);
}

void GameObject::AddComponent(std::shared_ptr<Mesh> component)
{
	component->gameObject = *this;
	components.push_back(component);
}

void GameObject::AddComponent(std::shared_ptr<Texture2D> component)
{
	component->gameObject = *this;
	components.push_back(component);
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

GameObject* GameObject::FindGO(std::string name, std::list<GameObject> gameObjectList)
{
	for (auto& go : gameObjectList)
	{
		if (go.name == name)
		{
			return &go;
		}
	}
	return nullptr;
}

void GameObject::UpdateComponents()
{
	for (auto& comp : components)
	{
		comp->Update();
	}
}
