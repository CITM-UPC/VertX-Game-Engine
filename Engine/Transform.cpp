#include "Transform.h"

Transform::Transform(GameObject& owner) : Component(owner)
{
	//Define Initial Values for GO Start
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	//scale = vec3(1, 1, 1);

	//Define Base Values
	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	//Identity Matrix for Manipulation
	referenceFrameMat = glm::mat3(1.0); 
}

Transform::~Transform() {}

//Moving GO to Space
void Transform::MoveTo(vec3 position, Space referenceFrame)
{
	vec3 resultingVector = position;

	if (referenceFrame == Space::LOCAL)
	{
		resultingVector = referenceFrameMat * position;
		this->position += resultingVector;
		return;
	}

	this->position = resultingVector;
}

//Moving GO incrementally -> shifting
void Transform::Move(vec3 displacement, Space referenceFrame)
{
	vec3 resultingVector = displacement;

	if (referenceFrame == Space::LOCAL)
		resultingVector = referenceFrameMat * displacement;

	position += resultingVector;
}

//Rotate Game Object via set Axis
void Transform::RotateTo(vec3f axis)
{
	//Set RefFrame to Global
	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	//Create RotationMatrix to set vals
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(axis.x)), -glm::sin(glm::radians(axis.x)),
		0, glm::sin(glm::radians(axis.x)), glm::cos(glm::radians(axis.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(axis.y)), 0, glm::sin(glm::radians(axis.y)),
		0, 1, 0,
		-glm::sin(glm::radians(axis.y)), 0, glm::cos(glm::radians(axis.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(axis.z)), -glm::sin(glm::radians(axis.z)), 0,
		glm::sin(glm::radians(axis.z)), glm::cos(glm::radians(axis.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply rotation matrix
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;

	referenceFrameMat[0].x = right.x;		referenceFrameMat[0].y = right.y;		referenceFrameMat[0].z = right.z;
	referenceFrameMat[1].x = up.x;			referenceFrameMat[1].y = up.y;			referenceFrameMat[1].z = up.z;
	referenceFrameMat[2].x = forward.x;		referenceFrameMat[2].y = forward.y;		referenceFrameMat[2].z = forward.z;
}

void Transform::Rotate(vec3f axis, Space referenceFrame)
{
	referenceFrameMat[0].x = right.x;		referenceFrameMat[0].y = right.y;		referenceFrameMat[0].z = right.z;
	referenceFrameMat[1].x = up.x;			referenceFrameMat[1].y = up.y;			referenceFrameMat[1].z = up.z;
	referenceFrameMat[2].x = forward.x;		referenceFrameMat[2].y = forward.y;		referenceFrameMat[2].z = forward.z;

	glm::vec3 vecInRefFrame = axis;
	if (referenceFrame == Space::LOCAL)
		vecInRefFrame = referenceFrameMat * axis;


	//Set the rotation matrix
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(vecInRefFrame.x)), -glm::sin(glm::radians(vecInRefFrame.x)),
		0, glm::sin(glm::radians(vecInRefFrame.x)), glm::cos(glm::radians(vecInRefFrame.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.y)), 0, glm::sin(glm::radians(vecInRefFrame.y)),
		0, 1, 0,
		-glm::sin(glm::radians(vecInRefFrame.y)), 0, glm::cos(glm::radians(vecInRefFrame.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.z)), -glm::sin(glm::radians(vecInRefFrame.z)), 0,
		glm::sin(glm::radians(vecInRefFrame.z)), glm::cos(glm::radians(vecInRefFrame.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;
}

void Transform::Update() {}