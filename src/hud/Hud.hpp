#pragma once
#include "engine/Application.hpp"
#include "engine/Window.hpp"
#include "opengl/Framebuffer.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"
#include <GLFW/glfw3.h>
#include "planets/Planet.hpp"
#include "engine/SpriteSheet.hpp"

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
	static Hud& Get()
	{
		static Hud instance;
		return instance;
	}

	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;

	void init(Window& window);
	void draw(GLFWwindow* window);
	void free();

	void bindFbo();
	void Hud::unbindFbo();

	float viewportWidth() const { return _viewportWidth; }
	float viewportHeight() const { return _viewportHeight; }

	void Hud::saveFile();
	void Hud::newFile();

	void setLowSliderSpeed();
	void setDefaultSliderSpeed();

	void ShowSettings();
	void ShowTerminal();


private:
	Hud() = default;
	~Hud() = default;

	void ShowMenuBar(GLFWwindow* window);
	void ShowSettingsWindow();
	void ShowViewportWindow();
	void ShowLogWindow();
	void ShowSaveAsWindow();
	void ShowNewSceneWindow();

	void ShowUpdateItem();

private:
	Framebuffer _fbo = Framebuffer();

	bool _saveFileOpen = false;
	bool _newFileOpen = false;
	bool _settingsOpen = true;
	bool _terminalOpen = true;
	bool _dockspaceOpen = true;


	char _bufferSaveLocation[20];
	int _sliderSpeed = 100;
	float _sliderSpeedDefault = 0.00005;

	std::shared_ptr<Planet> _planet = nullptr;
	std::shared_ptr<ShapeSettings> _shape = nullptr;
	std::shared_ptr<ColorSettings> _color = nullptr;

	SpriteSheet _loadingWheel = SpriteSheet("res/img/LoadingSheet.png", 31);

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;
};