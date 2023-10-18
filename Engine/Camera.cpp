#include "Camera.h"

Camera::Camera() : fov(70), aspectRatio(16.0 / 9.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), lookAtPos(0, 0, 0), camCenter(2, 2, 5), upVector	(0, 1, 0) {}