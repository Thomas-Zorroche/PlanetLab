#pragma once
#include "engine/Application.hpp"
#include "opengl/Framebuffer.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"
#include <GLFW/glfw3.h>

#include <memory>

class Window;
class Camera;
class NoiseSettings;
class ShapeSettings;
class ColorSettings;
class Planet;

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

	void draw(GLFWwindow* window);
	void init(GLFWwindow* window, const std::shared_ptr<Planet>& planet, float width, float height);
	void free();
	void bindFbo();
	void Hud::unbindFbo();

	float viewportWidth() const { return _viewportWidth; }
	float viewportHeight() const { return _viewportHeight; }

	/* UI Fonctions */
	bool wireframeMode() const { return _wireframeMode; }

private:
	Hud() = default;
	~Hud();

	Framebuffer _fbo = Framebuffer();

	static bool _wireframeMode;

	std::shared_ptr<Planet> _planet = nullptr;
	std::shared_ptr<ShapeSettings> _shape = nullptr;
	std::shared_ptr<ColorSettings> _color = nullptr;

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;
};