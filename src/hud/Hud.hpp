#pragma once
#include "engine/Application.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"

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

	/* UI Fonctions */
	bool wireframeMode() const { return _wireframeMode; }
	int resolution() const { return _resolution; }
	const Color& colorPlanet() const { return _planetColor; }

private:
	Hud() = default;
	~Hud() = default;

	static bool _wireframeMode;
	static int _resolution;
	static Color _planetColor;
	
};