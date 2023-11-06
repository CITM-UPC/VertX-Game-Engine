#pragma once
#include "Globals_ENGINE.h"
#include "glm/glm.hpp"
#include "Component.h"

class Transform : public Component
{
public:
	enum class Space
	{
		LOCAL,
		GLOBAL,
	};

	Transform();
	~Transform();

	//Moves the object to 'position'.
	void MoveTo(vec3 position, Space referenceFrame = Space::GLOBAL);

	//Moves the object in 'displacement' increments.
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL);

	//Rotates the object for its rotation to be the one given by 'axis'.
	void RotateTo(vec3f axis);

	//Rotates the object in 'axis' increments.
	void Rotate(vec3f axis, Space referenceFrame = Space::LOCAL);

	void Update() override;

	Type getType() const override {
		return Type::TRANSFORM;
	}

public:
	vec3 position;
	vec3 rotation;

	vec3 forward;
	vec3 right;
	vec3 up;

	glm::mat3x3 referenceFrameMat;

};