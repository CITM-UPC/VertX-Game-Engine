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

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

static void drawAABBox(const AABBox& aabb) {
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);

	glVec3(aabb.a());
	glVec3(aabb.b());
	glVec3(aabb.c());
	glVec3(aabb.d());
	glVec3(aabb.a());

	glVec3(aabb.e());
	glVec3(aabb.f());
	glVec3(aabb.g());
	glVec3(aabb.h());
	glVec3(aabb.e());
	glEnd();

	glBegin(GL_LINES);
	glVec3(aabb.h());
	glVec3(aabb.d());
	glVec3(aabb.f());
	glVec3(aabb.b());
	glVec3(aabb.g());
	glVec3(aabb.c());
	glEnd();
}

//AABBox GameObject::aabb() const {
//	AABBox aabbox;
//	if (_graphic.get()) aabbox = _graphic->aabb;
//	else if (children().empty()) {
//		aabbox.min = vec3(0);
//		aabbox.max = vec3(0);
//	}
//
//	for (const auto& child : children()) {
//		const auto child_aabb = (child.transform() * child.aabb()).AABB();
//		aabbox.min = glm::min(aabbox.min, child_aabb.min);
//		aabbox.max = glm::max(aabbox.max, child_aabb.max);
//	}
//
//	return aabbox;
//}

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
	drawAABBox(computeAABB());
}

AABBox GameObject::computeAABB()
{
	AABBox aabbox;

	Mesh* meshComponent = GetComponent<Mesh>();
	if (meshComponent != nullptr)
	{
		//aabbox = meshComponent->getAABB();
		const auto obBox = GetComponent<Transform>()->_transformationMatrix * aabbox;
		aabbox = obBox.AABB();
	}
	else
	{
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (const auto& child : childs)
	{
		const auto child_aabb = child->computeAABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}

	return aabbox;
}