#pragma once

#define _USE_MATH_DEFINES
#define NOMINAX // Avoid conflicts between min and max constants in Windef.h

#include <iostream>
#include <algorithm>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "engine/Application.hpp"

enum class DIRCAM
{
	FRONT = 0, LEFT
};

class Terrain;

class Camera
{
public:
	Camera();
	~Camera() {}


	// Getters Camera Data
	float GetFov() const { return _fov; }
	float GetWidth() const { return _widthScreen; }
	float GetHeight() const { return _heightScreen; }
	float GetNearPlane() const { return _nearPlane; }
	float GetFarPlane() const { return _farPlane; }
	
	void updateViewMatrix();
	glm::mat4 getViewMatrix() const { return _viewMatrix; }

	// Getters Camera
	float GetLastX() const  { return _lastX; }
	float GetLastY() const  { return _lastY; }
	float GetSensitivity() const  { return _sensitivity; }
	glm::vec3 GetPosition() const { return _Position; };
	
	glm::vec3 GetUpVector() const { return _UpVector; };
	glm::vec3 GetViewDir() const { return -glm::transpose(_viewMatrix)[2]; }
	glm::vec3 GetRightVector() const { return glm::transpose(_viewMatrix)[0]; }

	// Setters
	void SetLastX(float x) { _lastX = x; }
	void SetLastY(float y) { _lastY = y; }

	void updatePosition(glm::vec3 eye);


private:
	glm::mat4 _viewMatrix;
	glm::vec3 _Position;	  // Position of the camera

	float _phi;
	float _theta;

	float _distanceFromCenter = 4.0f;

	glm::vec3 _UpVector = glm::vec3(0, 1, 0);      // Up

	float _lastX;     // Last Mouse coordinates
	float _lastY;

	// Input Data
	float _sensitivity;

	// Technical Data
	float _fov = 45.0f;
	float _widthScreen = 1280.0;
	float _heightScreen = 720.0;
	float _nearPlane = 0.1f;
	float _farPlane = 5000.0f;
};