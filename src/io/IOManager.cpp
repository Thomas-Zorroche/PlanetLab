#include "IOManager.hpp"
#include "planets/Planet.hpp"

#include <iostream>
#include <filesystem>


bool IOManager::save(const std::string& outputFileName, const std::shared_ptr<Planet>& planet)
{
	// create a file instance
	std::string output = "res/scene/" + outputFileName + ".ini";
	mINI::INIFile file(output);

	// create a data structure
	mINI::INIStructure ini;

	// populate the structure
	ini["layers"]["count"] = std::to_string(planet->shapeSettings()->noiseLayers().size());

	int layerCount = 1;
	for (const auto& layer : planet->shapeSettings()->noiseLayers())
	{
		std::string section = "noiseSettings_" + std::to_string(layerCount);

		ini[section]["strength"] = std::to_string(layer->noiseSettings()->strength());
		ini[section]["layersCount"] = std::to_string(layer->noiseSettings()->layersCount());
		ini[section]["baseRoughness"] = std::to_string(layer->noiseSettings()->baseRoughness());
		ini[section]["roughness"] = std::to_string(layer->noiseSettings()->roughness());
		ini[section]["persistence"] = std::to_string(layer->noiseSettings()->persistence());
		ini[section]["centerX"] = std::to_string(layer->noiseSettings()->center().x);
		ini[section]["centerY"] = std::to_string(layer->noiseSettings()->center().y);
		ini[section]["centerZ"] = std::to_string(layer->noiseSettings()->center().z);
		ini[section]["minValue"] = std::to_string(layer->noiseSettings()->minValue());

		layerCount++;
	}

	// generate an INI file (overwrites any previous file)
	return file.generate(ini);
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
		std::string& strengthStr      = ini.get(section).get("strength");
		std::string& layersCountStr   = ini.get(section).get("layersCount");
		std::string& baseRoughnessStr = ini.get(section).get("baseRoughness");
		std::string& roughnessStr     = ini.get(section).get("roughness");
		std::string& centerXStr       = ini.get(section).get("centerX");
		std::string& centerYStr       = ini.get(section).get("centerY");
		std::string& centerZStr       = ini.get(section).get("centerZ");
		std::string& minValueStr      = ini.get(section).get("minValue");
	
		// Assign values
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->strength() = std::atof(strengthStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->layersCount() = std::atof(layersCountStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->baseRoughness() = std::atof(baseRoughnessStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->roughness() = std::atof(roughnessStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().x = std::atof(centerXStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().y = std::atof(centerYStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->center().z = std::atof(centerZStr.c_str());
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->minValue() = std::atof(minValueStr.c_str());
	}

	planet->update(ObserverFlag::NOISE);
}


