#include "GameObject.h"
#include <memory>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#define NOMINMAX
#include <Windows.h>
#include <string>
#include "Tree.hpp"

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

void GameObject::rotate(double degrees, const vec3& axis) {
	_transform = glm::rotate(_transform, glm::radians(degrees), axis);
}
void GameObject::translate(const vec3& dv) {
	_transform = glm::translate(_transform, dv);
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

AABBox GameObject::aabb() const {
	AABBox aabbox;
	if (_graphic.get()) aabbox = _graphic->aabb;
	/*else if (children().empty()) {
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (const auto& child : children()) {
		const auto child_aabb = (child.transform() * child.aabb()).AABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}*/

	return aabbox;
}

void GameObject::paint() const {

	glPushMatrix();
	glMultMatrixd(&_transform[0].x);

	glColor3ub(128, 0, 0);
	drawAABBox(aabb());

	if (_graphic.get()) _graphic->draw();
	/*for (auto& child : children()) child.paint();*/

	glPopMatrix();
}

void GameObject::Render(bool drawBoundingBox)
{
	bool toRender = true;
	// get necessary components
	TransformComponent* transform = GetComponent<TransformComponent>();
	if (GetComponent<MeshComponent>() == nullptr) {
		toRender = false;
	}

	glPushMatrix();
	glMultMatrixd(&transform->getTransform()[0].x);

	if (drawBoundingBox) {
		DrawBoundingBox(GetBoundingBox());
	}

	if (toRender) {
		MeshComponent* mesh = GetComponent<MeshComponent>();
		if (mesh->getMesh()) mesh->getMesh()->draw();
	}

	if (GetComponent<CameraComponent>() != nullptr) {
		GetComponent<CameraComponent>()->getCamera()->drawFrustum();
	}

	// render
	for (auto childIt = children.begin(); childIt != children.end(); ++childIt) {
		(*childIt)->Render(drawBoundingBox);
	}

	glPopMatrix();

}

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

void GameObject::DrawBoundingBox(const AABBox& aabb)
{
	glColor3ub(128, 0, 0);

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

AABBox GameObject::GetBoundingBox()
{
	AABBox aabbox;
	if (GetComponent<MeshComponent>() != nullptr) aabbox = GetComponent<MeshComponent>()->getMesh()->aabb;
	else if (children.empty()) {
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (auto i = children.begin(); i != children.end(); ++i) {
		const auto child_aabb = ((*i).get()->GetComponent<TransformComponent>()->getTransform() * (*i).get()->GetBoundingBox()).AABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}

	return aabbox;
}