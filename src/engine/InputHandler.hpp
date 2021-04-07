#pragma once 

#include "engine/Camera.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "GLFW/glfw3.h"

class Camera;
class Game;
struct CallbackPtr;

enum class ActiveKey {
	NONE, 
	ALT,
	MOUSE_LEFT,
	CTRL,
	S,
	N
};

class InputHandler
{
public:
	InputHandler() = default;
	
	void ProcessInput(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime);

	void SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr);

	bool canRotate() const;

private:
	void addKey(ActiveKey key);
	void removeKey(ActiveKey key);
	bool isKeyActive(ActiveKey key);

private:
	std::vector<ActiveKey> _activeKeys = { ActiveKey::NONE };

	bool _canInteract = false;
	bool _canSave = true;
	bool _canCreateNewFile = true;

	void Movement(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime);
};

//
// callbacks functions
// can't be object function due to glwf api
//
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

struct CallbackPtr
{
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<InputHandler> _inputHandler;

	CallbackPtr(const std::shared_ptr<Camera>& camera, const std::shared_ptr<InputHandler>& inputHandler)
		: _camera(camera), _inputHandler(inputHandler) {}
};