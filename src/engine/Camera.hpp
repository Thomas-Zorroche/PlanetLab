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


	void rotateUp(float angle);
	void rotateLeft(float angle);

	// Getters Camera Data
	float GetFov() const { return _fov; }
	float GetWidth() const { return _widthScreen; }
	float GetHeight() const { return _heightScreen; }
	float GetNearPlane() const { return _nearPlane; }
	float GetFarPlane() const { return _farPlane; }
	glm::mat4 getViewMatrix() const;
	// Getters Camera Gameplay
	float GetLastX() const  { return _lastX; }
	float GetLastY() const  { return _lastY; }
	float GetSensitivity() const  { return _sensitivity; }
	glm::vec3 GetPosition() const { return _Position; };
	glm::vec3 GetFrontVector() const { return _FrontVector; };

	// Setters
	void SetLastX(float x) { _lastX = x; }
	void SetLastY(float y) { _lastY = y; }


private:
	void computeDirectionVectors();


private:

	glm::vec3 _Position;	  // Position of the camera
	float _phi;
	float _theta;

	float _distanceFromCenter = 4.0f;

	glm::vec3 _FrontVector;   // Direction
	glm::vec3 _LeftVector;    // Left
	glm::vec3 _UpVector;      // Up

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