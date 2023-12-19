#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include <glm/vec3.hpp>


class Camera : public Component
{
public:

	Camera(GameObject* owner);
	Camera(const Camera& other);

	void Update() override;
	Type getType() const override {
		return Type::CAMERA;
	}

public:

	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	glm::dvec3 worldPosVec;		
	glm::dvec3 focusPosVec;		
	glm::dvec3 upVec;			

	double cameraSpeed;
	float cameraSpeedMultiplier;

	float cameraYaw;
	float cameraPitch;	
	float mouseSensitivity;

	const float zoomSpeed;

	glm::dvec3 lookAtPos;
	double camOffset;

	void ResetCameraParameters();
};