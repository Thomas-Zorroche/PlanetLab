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

/// Parameters of futur action after ask for saves before open / new file
struct SaveBeforeCloseParams
{
	SaveBeforeCloseParams(bool display = false, const std::string& action = "new", const std::string& pathToOpen = "")
		: display(display), action(action), pathToOpen(pathToOpen)
	{}

	bool display = false;
	std::string action = "new";
	std::string pathToOpen = "";
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
	void openFile(const std::string& filePath);
	void newFile();

	void setLowSliderSpeed() { _sliderSpeed = 0.001; }
	void setDefaultSliderSpeed() { _sliderSpeed = 0.08; }
	
	void toggleDisplaySettings();
	void toggleDisplayLog();

	void onResolutionUpdate(int resolution);

	void setWindowSize(int width, int height);

private:
	Editor() = default;
	~Editor() = default;

	// ========================================================
	// 	   Display Panel Functions
	// ========================================================
	void displayMenuBar(GLFWwindow* window);
	void displaySettings();
	void displayViewport();
	void displayLog();
	bool displayLaunchScreen();
	void displaySaveAsPopup();
	void displaySaveBeforeClosePopup();
	
	void drawUpdateModeItem();
	
	void updateNoiseLayersCount(int noiseLayersCountUpdated);

	void updateStatistics();

private:
	Framebuffer _fbo = Framebuffer();

	std::unique_ptr<EditorSettings> _editorSettings = std::make_unique<EditorSettings>();

	bool _displaySaveAsPopup = false;
	SaveBeforeCloseParams _saveBeforeCloseParams;

	bool _settingsOpen = true;
	bool _logOpen = true;
	bool _dockspaceOpen = true;
	bool _launchScreenOpen = true;

	char _bufferSaveLocation[20];

	float _sliderSpeed = 0.08;

	std::shared_ptr<Ceres::Planet> _planet = nullptr;

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;

	float _WIDTH;
	float _HEIGHT;

	MeshStatistics _statistics;

	std::unique_ptr<UiObserver> _observer = nullptr;

	std::vector<NoiseSettingsParameters> _noiseSettingsParameters;

	SpriteSheet _loadingWheel = SpriteSheet("res/img/LoadingSheet.png", 31);
	Texture _launchScreen = ResourceManager::Get().LoadTexture("res/img/launch_screen_0_8.png");

};

void prettyPrintNumber(int number, std::string& str);

}	
