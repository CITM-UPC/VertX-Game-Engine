#pragma once
#include "Globals_ENGINE.h"
#include "glm/glm.hpp"
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component
{
public:

	//Define Space between Local&Global - Mirrored from Unity Engine
	enum class Space
	{
		LOCAL,
		GLOBAL,
	};

	Transform(GameObject& owner);
	~Transform();

	//positioning
	void MoveTo(vec3 position, Space referenceFrame = Space::GLOBAL);

	//displacement
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL);

	//Rotates by axis
	void RotateTo(vec3f axis);

	//Rotates by axis increments
	void Rotate(vec3f axis, Space referenceFrame = Space::LOCAL);

	void Update() override;

	Type getType() const override {
		return Type::TRANSFORM;
	}

public:

	//Call by IMGUI for transformations - public
	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 forward;
	vec3 right;
	vec3 up;

	glm::mat3x3 referenceFrameMat;
};