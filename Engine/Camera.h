#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	Camera();

	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	glm::dvec3 worldPosVec;		// Represents the camera's location in world coordinates.
	glm::dvec3 focusPosVec;		// Represents the center of your view or the point you want the camera to be focused on.
	glm::dvec3 upVec;			// Represents the direction that you consider to be "up" in your world. It helps control the orientation of the camera. We do(0, 1, 0) to indicate that the y-axis is up.

	float cameraYaw = -90.0f;	// Yaw is initially pointing left
	float cameraPitch = 0.0f;	
	float mouseSensitivity = 0.25f;

	void ResetCameraParameters();
};