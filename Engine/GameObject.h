#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "BBox.hpp"
#include "Tree.hpp"
#include "Graphic.h"


class GameObject
{
public:

	std::string name;
	bool isActive = true;
	std::list<std::unique_ptr<Component>> components;

	std::list<std::unique_ptr<GameObject>> childs;

	GameObject* parent;

public:

	GameObject();
	~GameObject();
	

	template <typename GOC> GOC* GetComponent();

	std::list<std::unique_ptr<Component>>* GetComponents();

	void AddComponent(Component::Type component);

	template <typename GOC>
	void AddComponent(GOC& component);
	void RemoveComponent(Component::Type component);
	void UpdateComponents();
};

//Expand Template Info for GetComponent Function --> Improves consistency & Usability with multiple data formats
template<typename GOC>
inline GOC* GameObject::GetComponent()
{
	for (auto& component : components) {
		GOC* returnComponent = dynamic_cast<GOC*>(component.get());
		if (returnComponent) {
			return returnComponent;
		}
	}
	return nullptr;
}

template<typename GOC>
inline void GameObject::AddComponent(GOC& component)
{
	GOC copyOfComponent = component;
	components.emplace_back(std::make_unique<GOC>(std::move(copyOfComponent)));
}