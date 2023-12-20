#include "Transform.h"
#include "ModuleGameEngine.h"

Transform::Transform(GameObject* owner) : Component(owner)
{
	_transformationMatrix = mat4(1.0);
	_rotation = vec3(0);
	_scale = vec3(1);
}

Transform::Transform(GameObject* owner, mat4 transmat) : Component(owner)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_transformationMatrix[i][j] = transmat[i][j];
		}
	}

	//_transformationMatrix = transmat;
	_rotation = vec3(0);
	_scale = vec3(1);
}

Transform::~Transform() {}

//Moving GO to Space
void Transform::MoveTo(vec3 position)
{
	_position = position;
}

void Transform::Move(vec3 displacement, Space referenceFrame)
{
	glm::mat3 referenceFrameMat = (glm::mat4)_transformationMatrix;

	vec3 vecInRefFrame = displacement;
	if (referenceFrame == Space::GLOBAL)
		vecInRefFrame = displacement * referenceFrameMat;

	//Cut down on vector code by just creating a transform matrix, allowing for GLM rotation
	_transformationMatrix = glm::translate(_transformationMatrix, vecInRefFrame);
}

void Transform::RotateTo(vec3 rotVector)
{
	_right = vec3(1, 0, 0);
	_up = vec3(0, 1, 0);
	_forward = vec3(0, 0, 1);

	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(rotVector.x), vec3(1, 0, 0));
	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(rotVector.y), vec3(0, 1, 0));
	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(rotVector.z), vec3(0, 0, 1));

	_right *= _scale.x;
	_up *= _scale.y;
	_forward *= _scale.z;
}

//Rotate Game Object via set Axis
void Transform::RotateTo(double angle, vec3 axis)
{
	vec3 normalizedVec = glm::normalize(axis);

	_right = vec3(1, 0, 0);
	_up = vec3(0, 1, 0);
	_forward = vec3(0, 0, 1);

	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(angle), normalizedVec);
}

void Transform::Rotate(double angle, vec3 axis, Space referenceFrame)
{
	glm::mat3 referenceFrameMat = (glm::mat4)_transformationMatrix;

	vec3 vecInRefFrame = glm::normalize(axis);
	if (referenceFrame == Space::GLOBAL)
		vecInRefFrame = axis * referenceFrameMat;

	_transformationMatrix = glm::rotate(_transformationMatrix, glm::radians(angle), vecInRefFrame);
}

void Transform::Scale(vec3 scaleVector)
{
	RotateTo(vec3(_rotation.x, _rotation.y, _rotation.z));
}

void Transform::Update() {}