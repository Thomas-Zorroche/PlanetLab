#pragma once 

#include "engine/Camera.hpp"
#include <iostream>
#include <memory>
#include "GLFW/glfw3.h"

class Camera;
class Game;
struct CallbackPtr;

enum class ActiveKey {
	NONE, 
	C,
	E,	
	A	
};

class InputHandler
{
public:
	InputHandler() = default;
	
	void ProcessInput(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime);

	void SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr);

	ActiveKey GetActiveKey() { return _ActiveKey; };
	
	bool CanInteract() const { return _canInteract; }

	void SetCanInteract(bool interact) { _canInteract = interact; }

private:
	ActiveKey _ActiveKey = ActiveKey::NONE;

	bool _canInteract = false;

	void Movement(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime);
};

//
// callbacks functions
// can't be object function due to glwf api
//
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


struct CallbackPtr
{
	std::shared_ptr<Camera> _camera;

	CallbackPtr(const std::shared_ptr<Camera>& camera)
		: _camera(camera) {}
};