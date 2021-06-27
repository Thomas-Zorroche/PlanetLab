#include "IOManager.hpp"
#include "ceres/Planet.hpp"
#include "ceres/ShapeGenerator.hpp"
#include "ceres/noise/NoiseFilter.hpp"

#include <iostream>
#include <filesystem>

namespace PlanetLab
{

using namespace Ceres;

void IOManager::newFile()
{
	_currentFileSaved = false;
	_currentFileName = "New Scene";
	updateTitleWindow();
}


bool IOManager::save(std::shared_ptr<Planet>& planet)
{
	return _currentFileSaved ? saveAs(_currentFileName, planet) : false;
}

bool IOManager::saveAs(const std::string& outputFileName, std::shared_ptr<Planet>& planet)
{
	// create a file instance
	mINI::INIFile file(outputFileName);

	// create a data structure
	mINI::INIStructure ini;

	// populate the structure
	ini["layers"]["count"] = std::to_string(planet->shapeSettings()->noiseLayers().size());

	int layerCount = 1;
	for (const auto& layer : planet->shapeSettings()->noiseLayers())
	{
		std::string section = "noiseSettings_" + std::to_string(layerCount);

		std::string type = (layer->noiseSettings()->GetFilterType() == FilterType::Simple) ? "simple" : "rigid"; // TODO for the moment, there is just 2 type of noise...
		ini[section]["type"] = type;
		
		ini[section]["seed"] = std::to_string(planet->shapeGenerator()->noiseFilter(layerCount - 1)->Seed());

		ini[section]["strength"] = std::to_string(layer->noiseSettings()->strength());
		ini[section]["layersCount"] = std::to_string(layer->noiseSettings()->layersCount());
		ini[section]["baseRoughness"] = std::to_string(layer->noiseSettings()->baseRoughness());
		ini[section]["roughness"] = std::to_string(layer->noiseSettings()->roughness());
		ini[section]["persistence"] = std::to_string(layer->noiseSettings()->persistence());
		ini[section]["centerX"] = std::to_string(layer->noiseSettings()->center().x);
		ini[section]["centerY"] = std::to_string(layer->noiseSettings()->center().y);
		ini[section]["centerZ"] = std::to_string(layer->noiseSettings()->center().z);
		ini[section]["minValue"] = std::to_string(layer->noiseSettings()->minValue());
		ini[section]["weightMultiplier"] = std::to_string(layer->noiseSettings()->weightMultiplier());

		layerCount++;
	}

	// generate an INI file (overwrites any previous file)
	if (file.generate(ini))
	{
		_currentFileSaved = true;
		_currentFileName = outputFileName;
		_unsavedValues = false;
		updateTitleWindow();
	}
	else
	{
		_currentFileSaved = false;
		return false;
	}
}

bool IOManager::open(const std::string& inputFileName, std::shared_ptr<Planet>& planet)
{
	// first, create a file instance
	mINI::INIFile file(inputFileName);

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	// now we can read the file
	bool readSuccess = file.read(ini);
	if (!readSuccess)
	{
		return false;
	}

	// then import values and assign to the planet
	loadValues(ini, planet);

	_currentFileSaved = true;
	_currentFileName = inputFileName;
	_unsavedValues = false;
	updateTitleWindow();

	return true;
}

std::vector<std::string> IOManager::getAllFilesFromFolder(const std::string& path)
{
	std::vector<std::string> paths;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		paths.push_back(entry.path().string());
	}
	return paths;
}

void IOManager::loadValues(const mINI::INIStructure& ini, std::shared_ptr<Planet>& planet)
{
	std::string& layersCountStr = ini.get("layers").get("count");
	const int layersCount = std::atoi(layersCountStr.c_str());
	planet->updateNoiseLayersCount(layersCount);

	for (size_t i = 1; i <= layersCount; i++)
	{
		std::string section = "noiseSettings_" + std::to_string(i);
		// Read values
		std::string& typeStr		  = ini.get(section).get("type");
		std::string& seedStr		  = ini.get(section).get("seed");
		std::string& strengthStr      = ini.get(section).get("strength");
		std::string& layersCountStr   = ini.get(section).get("layersCount");
		std::string& baseRoughnessStr = ini.get(section).get("baseRoughness");
		std::string& roughnessStr     = ini.get(section).get("roughness");
		std::string& centerXStr       = ini.get(section).get("centerX");
		std::string& centerYStr       = ini.get(section).get("centerY");
		std::string& centerZStr       = ini.get(section).get("centerZ");
		std::string& minValueStr      = ini.get(section).get("minValue");
		std::string& weightMultiplierStr      = ini.get(section).get("weightMultiplier");
	
		// Assign values
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->GetFilterType() = (typeStr == "simple") ? FilterType::Simple : FilterType::Rigid;
		planet->shapeGenerator()->noiseFilter(i - 1)->Reseed(std::atof(seedStr.c_str()));
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->strength() = std::atof(strengthStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->layersCount() = std::atof(layersCountStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->baseRoughness() = std::atof(baseRoughnessStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->roughness() = std::atof(roughnessStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().x = std::atof(centerXStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().y = std::atof(centerYStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().z = std::atof(centerZStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->minValue() = std::atof(minValueStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->weightMultiplier() = std::atof(weightMultiplierStr.c_str());
	}

	planet->update(ObserverFlag::MESH);
}

void IOManager::updateTitleWindow()
{
	//if (_unsavedValues && _currentFileSaved)
	//{
	//	glfwSetWindowTitle(_windowPtr, std::string("Procedural Planets * " + _currentFileName).c_str());
	//}
	//else
	//{
	//	glfwSetWindowTitle(_windowPtr, std::string("Procedural Planets " + _currentFileName).c_str());
	//}
}

void IOManager::setUnsavedValues()
{
	if (!_unsavedValues && _currentFileSaved)
	{
		_unsavedValues = true;
		updateTitleWindow();
	}
}

}