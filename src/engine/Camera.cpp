#include "engine/Camera.hpp"
#include "common.hpp"


Camera::Camera()
	: _Position(-1, 0, -5), _phi(0), _theta(0), _CanTurn(false),
	_lastX(450.0f), _lastY(320.0f), _sensitivity(8.0f)
{
	computeDirectionVectors();
}

void Camera::Move(float deltaTime, DIRCAM direction)
{
	glm::vec3 dir;
	(direction == DIRCAM::FRONT) ? dir = _FrontVector : dir = _LeftVector;

	float dst = deltaTime * _Speed;

	MoveX(dst, dir);
	MoveZ(dst, dir);

	//_Position.y = Lerp<float>(_Position.y, _terrain->GetHeightOfTerrain(_Position.x, _Position.z) +_HeightCamera, abs(deltaTime) * _responsiveness);
	//_Position.y = glm::clamp(_Position.y, 12.0f, 100.0f);

	_cameraTime += abs(deltaTime);
	float offset_factor = sin(_cameraTime * _frequenceShake) * _amplitudeShake;

	computeDirectionVectors();
}


void Camera::rotateUp(float angle)
{
	_theta -= glm::radians(angle);
	computeDirectionVectors();
}
void Camera::rotateLeft(float angle)
{
	_phi -= glm::radians(angle);
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

void Camera::MoveX(float dst, const glm::vec3& dir)
{
	_Position.x += dst * dir.x;
}

void Camera::MoveZ(float dst, const glm::vec3& dir)
{
	_Position.z += dst * dir.z;
}
