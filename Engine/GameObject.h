#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Camera.h"

class GameObject
{
public:

	std::string name;
	bool isActive = true;
	std::list<std::unique_ptr<Component>> components;

	std::list<std::unique_ptr<GameObject>> childs;

	GameObject* parent;

	unsigned long UUID;

public:

	GameObject();
	~GameObject();

	template <typename T> T* GetComponent();
	

	std::list<std::unique_ptr<Component>>* GetComponents();

	void Move(GameObject* newParent, std::list<unique_ptr<GameObject>>& listToCheck);
	void removeChild(GameObject* childToRemove);

	void AddComponent(Component::Type component);
	

	template <typename T>
	void AddComponent(T& component);

	void RemoveComponent(Component::Type component);

	void UpdateComponents();

	void RenderComponents();

	mat4 getGlobalTransform();

	AABBox computeAABB();
	void drawAABBox(const AABBox& aabb);
};

template<typename T>
inline T* GameObject::GetComponent()
{
	for (auto& component : components) {
		T* returnComponent = dynamic_cast<T*>(component.get());
		if (returnComponent) {
			return returnComponent;
		}
	}
	return nullptr;
}

template<typename T>
inline void GameObject::AddComponent(T& component)
{
	T copyOfComponent = component; // Make a copy of the component
	components.emplace_back(std::make_unique<T>(std::move(copyOfComponent)));
}