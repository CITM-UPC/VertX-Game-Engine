#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"

class GameObject
{
public:

	std::string name;
	bool isActive = true;

private:

	std::vector<std::shared_ptr<Component>> components;

public:

	GameObject();
	~GameObject();

	template <typename GOC> GOC* GetComponent();
	std::vector<std::shared_ptr<Component>> GetComponents();
	void AddComponent(Component::Type component);
	void AddComponent(std::shared_ptr<Mesh> component);
	void AddComponent(std::shared_ptr<Texture2D> component);
	void RemoveComponent(Component::Type component);

	//Sort Through GOList
	static GameObject* FindGO(std::string name, std::list<GameObject> gameObjectList);

	void UpdateComponents();
};

//Expand Template Info for GetComponent Function --> Improves consistency & Usability with multiple data formats
template<typename GOC>
inline GOC* GameObject::GetComponent()
{
	for (auto component : components) {
		GOC* returnComponent = dynamic_cast<GOC*>(component.get());
		if (returnComponent) {
			return returnComponent;
		}
	}
	return nullptr;
}