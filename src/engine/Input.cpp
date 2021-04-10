#include "engine/Input.hpp"
#include "engine/Application.hpp"

#include <GLFW/glfw3.h>
#include <iostream>


bool Input::IsKeyPressed(KeyCode key)
{
	auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, static_cast<int32_t>(key));
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	return false;
}

glm::vec2 Input::GetMousePosition()
{
	return glm::vec2();
}

float Input::GetMouseX()
{
	return 0.0f;
}

float Input::GetMouseY()
{
	return 0.0f;
}
