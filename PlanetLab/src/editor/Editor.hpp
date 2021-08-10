#pragma once
#include "editor/Application.hpp"
#include "engine/Window.hpp"
#include "engine/opengl/Framebuffer.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"
#include <GLFW/glfw3.h>
#include "engine/SpriteSheet.hpp"
#include "engine/opengl/Texture.h"
#include "engine/ResourceManager.hpp"

#include "UiObserver.hpp"

#include "Ceres/Planet.hpp"

#include "editor/NoiseSettingsParameters.h"


namespace Ceres
{
class NoiseSettings;
class ShapeSettings;
class ColorSettings;
}

namespace PlanetLab
{
class Window;
class Camera;
class Application;



struct MeshStatistics
{
	std::string verticesCount = "";
	std::string facesCount = "";
	std::string trianglesCount= "";
};


class Editor
{
public:
	static Editor& Get()
	{
		static Editor instance;
		return instance;
	}

	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;

	void init(Window& window);
	void draw(GLFWwindow* window);
	void free();

	void bindFbo();
	void unbindFbo();

	std::unique_ptr<EditorSettings>& getEditorSettings() { return _editorSettings; }

	float viewportWidth() const { return _viewportWidth; }
	float viewportHeight() const { return _viewportHeight; }

	void saveFile();

	void openNewFilePopup() { _newFilePopupOpen = true; }

	// TODO Re implement
	void setLowSliderSpeed() { _sliderSpeed = 1; }
	void setDefaultSliderSpeed() { _sliderSpeed = 100; }
	// --------------------------

	void toggleDisplaySettings();
	void toggleDisplayLog();

	void onResolutionUpdate(int resolution);

	void setWindowSize(int width, int height);

private:
	Editor() = default;
	~Editor() = default;

	// ========================================================
	// 	   Display Window Functions
	// ========================================================
	void displayMenuBar(GLFWwindow* window);
	void displaySettings();
	void displayViewport();
	void displayLog();
	bool displayLaunchScreen();
	void displaySaveAsPopup();
	void displayNewScenePopup();

	
	void drawUpdateModeItem();
	
	void updateNoiseLayersCount(int noiseLayersCountUpdated);

	void updateStatistics();

private:
	Framebuffer _fbo = Framebuffer();

	std::unique_ptr<EditorSettings> _editorSettings = std::make_unique<EditorSettings>();

	bool _saveFilePopupOpen = false;
	bool _newFilePopupOpen = false;
	bool _settingsOpen = true;
	bool _logOpen = true;
	bool _dockspaceOpen = true;
	bool _launchScreenOpen = true;

	char _bufferSaveLocation[20];
	int _sliderSpeed = 100;
	float _sliderSpeedDefault = 0.00005;

	std::shared_ptr<Ceres::Planet> _planet = nullptr;
	std::shared_ptr<Ceres::ShapeSettings> _shape = nullptr;
	std::shared_ptr<Ceres::ColorSettings> _color = nullptr;

	SpriteSheet _loadingWheel = SpriteSheet("res/img/LoadingSheet.png", 31);

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;

	MeshStatistics _statistics;

	std::unique_ptr<UiObserver> _observer = nullptr;

	std::vector<NoiseSettingsParameters> _noiseSettingsParameters;

	Texture _launchScreen = ResourceManager::Get().LoadTexture("res/img/launch_screen_0_8.png");
};

void prettyPrintNumber(int number, std::string& str);

}	
