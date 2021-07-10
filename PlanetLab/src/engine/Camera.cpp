#include "engine/Camera.hpp"
#include "common.hpp"

namespace PlanetLab
{

Camera::Camera()
	: _Position(0, 0, +_distanceFromCenter),
	_lastX(450.0f),
	_lastY(320.0f)
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

void Camera::zoom(float value)
{
	_Position *= value;
	updateViewMatrix();
}

}	// ns editor




