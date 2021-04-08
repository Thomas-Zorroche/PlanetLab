#pragma once
#include "engine/Application.hpp"
#include "opengl/Framebuffer.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"
#include <GLFW/glfw3.h>
#include "planets/Planet.hpp"

#include <memory>
#include <string>

class Window;
class Camera;
class NoiseSettings;
class ShapeSettings;
class ColorSettings;

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

	void Hud::saveFile();
	void Hud::newFile();

	/* UI Fonctions */
	bool wireframeMode() const { return _wireframeMode; }

	void generateUpdateQueue(bool onRelease = false);
private:
	Hud() = default;
	~Hud();

	void update(ObserverFlag flag);
	void addUpdateIntoQueue(ObserverFlag flag);
	
private:
	Framebuffer _fbo = Framebuffer();

	std::string _consoleBuffer = "";

	static bool _wireframeMode;
	bool _saveFileOpen = false;
	bool _newFileOpen = false;
	int _updateMode = 0;

	std::shared_ptr<Planet> _planet = nullptr;
	std::shared_ptr<ShapeSettings> _shape = nullptr;
	std::shared_ptr<ColorSettings> _color = nullptr;

	std::vector<ObserverFlag> _updatesQueue;

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;
};