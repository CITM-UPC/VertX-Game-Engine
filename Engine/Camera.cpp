#include "Camera.h"

Camera::Camera() : fov(70), 
				aspectRatio(16.0 / 9.0f), 
				clippingPlaneViewNear(0.1), 
				clippingPlaneViewFar(150), 
				worldPosVec (2.0f, 2.0f, 5.0f),
				focusPosVec (0.0f, 0.0f, 0.0f), 
				upVec (0.0f, 1.0f, 0.0f){}

void Camera::ResetCameraParameters() 
{
	fov = 70;
	aspectRatio = 16.0 / 9.0;
	clippingPlaneViewNear = 0.1;
	clippingPlaneViewFar = 150;
	
	worldPosVec.x = 2.0f;
	worldPosVec.y = 2.0f;
	worldPosVec.z = 5.0f;

	focusPosVec.x = 0.0f;
	focusPosVec.y = 0.0f;
	focusPosVec.z = 0.0f;

	upVec.x = 0.0f;
	upVec.y = 1.0f;
	upVec.z = 0.0f;
}