#include "engine/Camera.hpp"
#include "common.hpp"


Camera::Camera()
	: _Position(-1, 0, -5), 
	_phi(0), 
	_theta(0),
	_lastX(450.0f), 
	_lastY(320.0f), 
	_sensitivity(1.0f)
{
	computeDirectionVectors();
}

glm::mat4 Camera::getViewMatrix() const
{ 
	float eyeX = _distanceFromCenter * glm::cos(_phi) * glm::sin(_theta);
	float eyeY = _distanceFromCenter * glm::sin(_phi) * glm::sin(_theta);
	float eyeZ = _distanceFromCenter * glm::cos(_theta);

	return glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), glm::vec3(0), glm::vec3(0, 1, 0)); 
}


void Camera::rotateUp(float angle)
{
	_theta += glm::radians(angle);
	computeDirectionVectors();
}
void Camera::rotateLeft(float angle)
{
	_phi += glm::radians(angle);
	computeDirectionVectors();
}


/*
* Private Functions
*/
void Camera::computeDirectionVectors()
{
	// Direction
	_FrontVector = glm::vec3(glm::cos(glm::radians(_theta)) * glm::sin(glm::radians(_phi)),
		glm::sin(glm::radians(_theta)),
		glm::cos(glm::radians(_theta)) * glm::cos(glm::radians(_phi)));

	// Left
	_LeftVector = glm::vec3((glm::sin(glm::radians(_phi) + (M_PI / 2))),
		0,
		glm::cos(glm::radians(_phi) + (M_PI / 2)));

	// Up
	_UpVector = glm::cross(_FrontVector, _LeftVector);
}
