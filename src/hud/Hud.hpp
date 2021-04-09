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
class Application;
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

	void init(GLFWwindow* window, const std::shared_ptr<Application>& app, float width, float height);
	void draw(GLFWwindow* window);
	void free();

	void bindFbo();
	void Hud::unbindFbo();

	float viewportWidth() const { return _viewportWidth; }
	float viewportHeight() const { return _viewportHeight; }

	void Hud::saveFile();
	void Hud::newFile();

private:
	Hud() = default;
	~Hud() = default;

	void ShowMenuBar(GLFWwindow* window);
	void ShowSettingsWindow();
	void ShowViewportWindow();
	void ShowConsoleWindow();
	void ShowSaveAsWindow();
	void ShowNewSceneWindow();
	
private:
	Framebuffer _fbo = Framebuffer();

	std::string _consoleBuffer = "";

	bool _saveFileOpen = false;
	bool _newFileOpen = false;
	bool _dockspaceOpen = true;
	char _bufferSaveLocation[20];

	std::shared_ptr<Application> _app = nullptr;
	std::shared_ptr<Planet> _planet = nullptr;
	std::shared_ptr<ShapeSettings> _shape = nullptr;
	std::shared_ptr<ColorSettings> _color = nullptr;


	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;
};