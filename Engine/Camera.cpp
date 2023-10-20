#include "Camera.h"

Camera::Camera() : fov(70), 
				aspectRatio(16.0 / 9.0f), 
				clippingPlaneViewNear(0.1), 
				clippingPlaneViewFar(150), 
				camCenterPos(2.0f, 2.0f, 5.0f),
				lookAtPos (0.0f, 0.0f, 0.0f), 
				upVector (0.0f, 1.0f, 0.0f){}

void Camera::ResetCameraParameters() 
{
	fov = 70;
	aspectRatio = 16.0 / 9.0;
	clippingPlaneViewNear = 0.1;
	clippingPlaneViewFar = 150;
	
	camCenterPos.x = 2.0f;
	camCenterPos.y = 2.0f;
	camCenterPos.z = 5.0f;

	lookAtPos.x = 0.0f;
	lookAtPos.y = 0.0f;
	lookAtPos.z = 0.0f;

	upVector.x = 0.0f;
	upVector.y = 1.0f;
	upVector.z = 0.0f;
}