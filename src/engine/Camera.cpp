#include "engine/Camera.hpp"
#include "common.hpp"


Camera::Camera()
	: _Position(0, 0, -4),
	_phi(0), 
	_theta(0),
	_lastX(450.0f), 
	_lastY(320.0f), 
	_sensitivity(1.0f)
{
	updateViewMatrix();
}

void Camera::updateViewMatrix()
{ 
	_viewMatrix = glm::lookAt(_Position, glm::vec3(0, 0, 0), _UpVector);
}

void Camera::updatePosition(glm::vec3 eye)
{
	_Position = std::move(eye);
	updateViewMatrix();
}



