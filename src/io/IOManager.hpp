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

	bool save(const std::shared_ptr<Planet>& planet);
	bool saveAs(const std::string& outputFileName, const std::shared_ptr<Planet>& planet);
	bool open(const std::string& inputFileName, std::shared_ptr<Planet>& planet);
	void newFile();

	std::vector<std::string> getAllFilesFromFolder(const std::string& path);

	bool currentFileSaved() const { return _currentFileSaved; }
	std::string currentFileName() const { return _currentFileName; }

	void updateTitleWindow(GLFWwindow* window);

private:
	void loadValues(const mINI::INIStructure& ini, std::shared_ptr<Planet>& planet);

	IOManager() = default;
	~IOManager() = default;

private:
	bool _currentFileSaved = false;
	std::string _currentFileName = "New Scene";
};