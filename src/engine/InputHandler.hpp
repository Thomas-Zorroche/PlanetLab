#pragma once 

#include "engine/Camera.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "GLFW/glfw3.h"

class Camera;
struct CallbackPtr;

enum class ActiveKey {
	ALT,
	MOUSE_LEFT,
	CTRL,
	SHIFT,
	S,
	N
};

class InputHandler
{
public:
	InputHandler() = default;
	
	void SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr);
	
	void ClickMouseButton();

	bool IsMouseButtonPressed() const { return _mouseButtonPressed; }

private:
	bool _mouseButtonPressed = false;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

struct CallbackPtr
{
	std::shared_ptr<Camera> _camera;

	CallbackPtr(const std::shared_ptr<Camera>& camera)
		: _camera(camera) {}
};