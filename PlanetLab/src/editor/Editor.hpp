#pragma once
#include "editor/Application.hpp"
#include "engine/Window.hpp"
#include "engine/opengl/Framebuffer.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"
#include <GLFW/glfw3.h>
#include "engine/opengl/Texture.h"
#include "engine/ResourceManager.hpp"
#include "editor/EditorSettings.hpp"

#include "UiObserver.hpp"

#include "Ceres/Planet.hpp"

#include "editor/panels/SettingsPanel.hpp"
#include "editor/panels/Viewer3DPanel.hpp"
#include "editor/panels/Viewer2DPanel.hpp"


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
	void render();
	void free();

	const std::unique_ptr<EditorSettings>& getEditorSettings() { return _editorSettings; }

	float viewportWidth() const { return  _viewer3DPanel.getViewportWidth(); }
	float viewportHeight() const { return _viewer3DPanel.getViewportHeight(); }

	void saveFile();
	void openFile(const std::string& filePath);
	void newFile();

	void setLowSliderSpeed() { _settingsPanel.setSliderSpeed(0.001); }
	void setDefaultSliderSpeed() { _settingsPanel.setSliderSpeed(0.08); }
	
	void toggleDisplaySettings();
	void toggleDisplayLog();
	void toggleDisplayViewer2D();

	void onResolutionUpdate(int resolution);
	void onMeshUpdate();

	void setWindowSize(int width, int height);

	void bindFbo() { _viewer3DPanel.bindFbo(); }
	void unbindFbo() { _viewer3DPanel.unbindFbo(); }

	bool isViewer3DHovered() const { return _viewer3DPanel.isViewer3DHovered(); }

	unsigned int getSelectedLayerId() const { return _selectedLayerId; }
	void setSelectedLayerId(unsigned int id) { _selectedLayerId = id; _viewer2DPanel.updateTexture(); }

	const std::string& getLayerSelectedName() const { return _settingsPanel.getLayer(_selectedLayerId).getName(); }
	
	bool hasNoiseLayer() const { return _settingsPanel.getNoiseLayersCount(); }

	bool isInputTextFocused(std::string id) const { return id == _idInputTextFocused; }
	void setInputTextFocused(std::string id) { _idInputTextFocused = id; }

	bool canUseShortcuts() const { return _idInputTextFocused == ""; }

private:
	Editor() = default;
	~Editor() = default;

	// ========================================================
	// 	   Display Panel Functions
	// ========================================================
	void displayMenuBar(GLFWwindow* window);
	void displayLog();
	bool displayLaunchScreen();
	void displaySaveAsPopup();
	void displaySaveBeforeClosePopup();


private:
	bool _displaySaveAsPopup = false;
	SaveBeforeCloseParams _saveBeforeCloseParams;

	bool _settingsOpen = true;
	bool _logOpen = true;
	bool _viewer2DOpen = false;
	bool _dockspaceOpen = true;
	bool _launchScreenOpen = true;

	char _bufferSaveLocation[20];

	std::shared_ptr<Ceres::Planet> _planet = nullptr;

	float _WIDTH = 1920.0f;
	float _HEIGHT = 1080.0f;


	float _settingsWidth = 200.0f;

	std::unique_ptr<UiObserver> _observer = nullptr;

	Texture _launchScreen = ResourceManager::Get().LoadTexture("res/img/launch_screen_0_8.png");

	SettingsPanel _settingsPanel;
	Viewer3DPanel _viewer3DPanel;
	Viewer2DPanel _viewer2DPanel;

	std::unique_ptr<EditorSettings> _editorSettings = std::make_unique<EditorSettings>();

	unsigned int _selectedLayerId = 0;

	std::string _idInputTextFocused = "";

	bool _canUseShortcurts = true;
};


}	
