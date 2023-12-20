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

	Transform(GameObject* owner);
	Transform(GameObject* owner, mat4 transmat);

	~Transform();

	//positioning
	void MoveTo(vec3 position);

	//displacement
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL);

	//Rotates by axis
	void RotateTo(double angle, vec3 axis);
	void RotateTo(vec3 axis);

	//Rotates by axis increments
	void Rotate(double angle, vec3 axis, Space referenceFrame = Space::LOCAL);

	void Scale(vec3 scaleVector);

	void Update() override;

	Type getType() const override {
		return Type::TRANSFORM;
	}

public:
	union
	{
		mat4 _transformationMatrix;
		struct
		{
			vec3 _right;	double _padding1;
			vec3 _up;		double _padding2;
			vec3 _forward;	double _padding3;
			vec3 _position;	double _padding4;
		};
	};

	vec3 _rotation;
	vec3 _scale;

public:

	inline const mat4& transform() const { return _transformationMatrix; }
	inline const vec3& position() const { return _position; }
	inline const vec3& pos() const { return _position; }

	inline const vec3& right() const { return _right; }
	inline const vec3& up() const { return _up; }
	inline const vec3& forward() const { return _forward; }
};