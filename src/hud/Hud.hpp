#pragma once
#include "engine/Application.hpp"
#include "glm/glm.hpp"

#include <memory>

class Camera;
class Window;
class Camera;

class Hud
{
public:
	static Hud& get()
	{
		static Hud instance;
		return instance;
	}

	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;

	void draw(const std::shared_ptr<Camera>& camera, const Window& windowObject) const;
	void init(GLFWwindow* window);
	void free();

private:
	Hud() = default;
	~Hud() = default;
	
};