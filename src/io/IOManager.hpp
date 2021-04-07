#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ini.h"
#include "engine/Application.hpp"


class Planet;
class GLFWwindow;

class IOManager
{
public:
	static IOManager& get()
	{
		static IOManager instance;
		return instance;
	}

	IOManager(const IOManager&) = delete;
	IOManager& operator=(const IOManager&) = delete;

	void setWindowPtr(GLFWwindow* window) { _windowPtr = window; }
	void setPlanetPtr(const std::shared_ptr<Planet>& planet) { _planet = planet; }

	bool save();
	bool saveAs(const std::string& outputFileName);
	bool open(const std::string& inputFileName);
	void newFile();

	std::vector<std::string> getAllFilesFromFolder(const std::string& path);

	bool currentFileSaved() const { return _currentFileSaved; }
	std::string currentFileName() const { return _currentFileName; }
	void setUnsavedValues();

	void updateTitleWindow();

private:
	void loadValues(const mINI::INIStructure& ini);

	IOManager() = default;
	~IOManager() = default;

private:
	bool _currentFileSaved = false;				// If user has already save once or open a scene
	bool _unsavedValues = false;				// If user change any value and did not save
	std::string _currentFileName = "New Scene";

	GLFWwindow* _windowPtr = nullptr;
	std::shared_ptr<Planet> _planet = nullptr;
};