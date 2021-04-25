#pragma once

#include <string>
#include <memory>
#include <vector>
//#include <GLFW/glfw3.h>

#include "ini.h"
#include "engine/Application.hpp"

namespace proceduralPlanet
{
class Planet;
}


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

	//void setWindowPtr(GLFWwindow* window) { _windowPtr = window; }

	bool save(std::shared_ptr<proceduralPlanet::Planet>& planet);
	bool saveAs(const std::string& outputFileName, std::shared_ptr<proceduralPlanet::Planet>& planet);
	bool open(const std::string& inputFileName, std::shared_ptr<proceduralPlanet::Planet>& planet);
	void newFile();

	std::vector<std::string> getAllFilesFromFolder(const std::string& path);

	bool currentFileSaved() const { return _currentFileSaved; }
	std::string currentFileName() const { return _currentFileName; }
	void setUnsavedValues();

	void updateTitleWindow();

private:
	void loadValues(const mINI::INIStructure& ini, std::shared_ptr<proceduralPlanet::Planet>& planet);

	IOManager() = default;
	~IOManager() = default;

private:
	bool _currentFileSaved = false;				// If user has already save once or open a scene
	bool _unsavedValues = false;				// If user change any value and did not save
	std::string _currentFileName = "New Scene";

	//GLFWwindow* _windowPtr = nullptr;
};

