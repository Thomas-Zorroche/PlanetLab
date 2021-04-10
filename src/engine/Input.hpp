#pragma once

#include "glm/glm.hpp"


enum class KeyCode
{
	N = 78,
	S = 83,
	T = 84,

	SHIFT = 340,
	CTRL = 341,
	ALT = 342,
};

enum class MouseCode
{
	MouseLeft = 0,
};

class Input
{
public:
	static bool IsKeyPressed(KeyCode key);
	
	static bool IsMouseButtonPressed(MouseCode button);
	static glm::vec2 GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();

private:
	//static bool _keyPressed = false;
};