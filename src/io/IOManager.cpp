#include "IOManager.hpp"
#include "planets/Planet.hpp"


void IOManager::save(const std::string& outputFileName)
{

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
