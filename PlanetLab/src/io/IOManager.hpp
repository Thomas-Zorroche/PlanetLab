#pragma once

#include "ini.h"
#include "glm/glm.hpp"
#include "editor/Application.hpp"

#include "Ceres/color/Color.hpp"

namespace Ceres
{
class Planet;
}

namespace PlanetLab
{

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

	bool save(std::shared_ptr<Ceres::Planet>& planet);
	bool saveAs(const std::string& outputFileName, std::shared_ptr<Ceres::Planet>& planet);
	bool open(const std::string& inputFileName, std::shared_ptr<Ceres::Planet>& planet);
	void newFile();

	std::vector<std::string> getAllFilesFromFolder(const std::string& path);

	bool isFileOnDisk() const { return _fileOnDisk; }
	std::string currentFileName() const { return _currentFileName; }
	void setFileValuesSaved(bool save) { _fileValuesSaved = save; updateTitleWindow(); }

	/// Whether there are unsaved values
	bool areUnsavedValues() const { return /*_fileOnDisk && */!_fileValuesSaved; }


	void updateTitleWindow();

private:
	void loadValues(mINI::INIStructure& ini, std::shared_ptr<Ceres::Planet>& planet);

	// Read vec3 string from ini files and return Color
	Ceres::Color parseColor(const std::string& vec3String);

	// Read vec3 string from ini files and return glm::vec3
	glm::vec3 parseVec3(const std::string& vec3String);

	// Write Color into vec3 string
	std::string writeColor(const PlanetLab::Color& color);
	std::string writeColor(const float* color);

	IOManager() = default;
	~IOManager() = default;

private:
	bool _fileOnDisk = false;				// If user has already save once or open a scene
	bool _fileValuesSaved = false;			// If there are no more unsaved changes
	std::string _currentFileName = "New Scene";
};

}

