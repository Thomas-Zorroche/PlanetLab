#include "IOManager.hpp"
#include "ceres/Planet.hpp"
#include "ceres/ShapeGenerator.hpp"
#include "ceres/noise/NoiseFilter.hpp"

#include <iostream>
#include <filesystem>
#include <sstream>

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

		std::string type = (layer->noiseSettings()->GetFilterType() == FilterType::Simple) ? "simple" : "rigid"; // TODO for the moment, there are just 2 type of noise...
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

	// Save color settings
	ini["colorSettings"]["useOceanColor"] = std::to_string(planet->colorSettings()->GetUseOceanColor());
	ini["colorSettings"]["oceanDepth"] = std::to_string(planet->colorSettings()->GetOceanDepth());
	ini["colorSettings"]["oceanColor"] = writeColor(planet->colorSettings()->GetOceanColor());
	ini["colorSettings"]["planetColor"] = writeColor(planet->colorSettings()->color());
	ini["colorSettings"]["useLandmassRamp"] = std::to_string(planet->colorSettings()->GetUseLandmassRamp());
	
	// Save Gradient Settings
	const auto& marks = planet->colorSettings()->GetGradient().getMarks();
	int marksCount = marks.size();
	ini["gradientSettings"]["marksCount"] = std::to_string(marksCount);
	int index = 1;
	for (const auto& mark : marks)
	{
		std::string labelPos = "pos_" + std::to_string(index);
		std::string labelColor = "color_" + std::to_string(index);
		ini["gradientSettings"][labelPos] = std::to_string(mark->position);
		ini["gradientSettings"][labelColor] = writeColor(mark->color);
		index++;
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
		std::string& typeStr		      = ini.get(section).get("type");
		std::string& seedStr		      = ini.get(section).get("seed");
		std::string& strengthStr          = ini.get(section).get("strength");
		std::string& layersCountStr       = ini.get(section).get("layersCount");
		std::string& baseRoughnessStr     = ini.get(section).get("baseRoughness");
		std::string& roughnessStr         = ini.get(section).get("roughness");
		std::string& centerXStr           = ini.get(section).get("centerX");	// TODO Replace with parse Vec3 function
		std::string& centerYStr           = ini.get(section).get("centerY");
		std::string& centerZStr           = ini.get(section).get("centerZ");
		std::string& minValueStr          = ini.get(section).get("minValue");
		std::string& weightMultiplierStr  = ini.get(section).get("weightMultiplier");
	
		// Assign values
		planet->shapeSettings()->noiseLayer(i - 1)->noiseSettings()->GetFilterType() = (typeStr == "simple") ? FilterType::Simple : FilterType::Rigid;
		planet->shapeGenerator()->updateFilterType(i - 1);
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

	// Load Color values
	std::string& useoceancolorStr = ini.get("colorsettings").get("useoceancolor");
	std::string& oceandepthStr = ini.get("colorsettings").get("oceandepth");
	std::string& oceancolorStr = ini.get("colorsettings").get("oceancolor");
	std::string& planetcolorStr = ini.get("colorsettings").get("planetcolor");
	std::string& uselandmassrampStr = ini.get("colorsettings").get("uselandmassramp");

	planet->colorSettings()->GetUseOceanColor() = std::atoi(useoceancolorStr.c_str());
	planet->colorSettings()->GetOceanDepth() = std::atoi(oceandepthStr.c_str());
	planet->colorSettings()->GetOceanColor() = parseColor(oceancolorStr);
	planet->colorSettings()->color() = parseColor(planetcolorStr);
	planet->colorSettings()->GetUseLandmassRamp() = std::atoi(oceandepthStr.c_str());

	// Load Gradient values
	std::string& marksCountStr = ini.get("gradientsettings").get("markscount");
	int marksCount = std::atof(marksCountStr.c_str());
	std::list<ImGradientMark> marks;
	for (size_t i = 1; i <= marksCount; i++)
	{
		std::string labelPos = "pos_" + std::to_string(i);
		std::string labelColor = "color_" + std::to_string(i);

		std::string& posStr = ini.get("gradientsettings").get(labelPos);
		std::string& colorStr = ini.get("gradientsettings").get(labelColor);

		marks.push_back(ImGradientMark( parseVec3(colorStr), std::atof(posStr.c_str()) ));
	}
	planet->colorSettings()->setColorMarks(marks);

	planet->update(ObserverFlag::MESH);
	planet->update(ObserverFlag::COLOR);
}

void IOManager::updateTitleWindow()
{
	if (_unsavedValues && _currentFileSaved)
	{
		Application::Get().updateTitle(std::string("Procedural Planets * " + _currentFileName));
	}
	else
	{
		Application::Get().updateTitle(std::string("Procedural Planets " + _currentFileName));
	}
}

void IOManager::setUnsavedValues()
{
	if (!_unsavedValues && _currentFileSaved)
	{
		_unsavedValues = true;
		updateTitleWindow();
	}
}

PlanetLab::Color IOManager::parseColor(const std::string& vec3String)
{
	if (vec3String == "") return PlanetLab::Color();

	std::vector<float> color;
	std::string value;
	std::istringstream stream(vec3String);
	while (std::getline(stream, value, ' '))
	{
		color.push_back(std::atof(value.c_str()));
	}

	return PlanetLab::Color(color[0], color[1], color[2]);
}

glm::vec3 IOManager::parseVec3(const std::string& vec3String)
{
	if (vec3String == "") return glm::vec3();

	std::vector<float> vec3;
	std::string value;
	std::istringstream stream(vec3String);
	while (std::getline(stream, value, ' '))
	{
		vec3.push_back(std::atof(value.c_str()));
	}

	return glm::vec3(vec3[0], vec3[1], vec3[2]);
}


std::string IOManager::writeColor(const PlanetLab::Color& color)
{
	return std::to_string(color.r) + " " + std::to_string(color.g) + " " + std::to_string(color.b);
}

std::string IOManager::writeColor(const float* color)
{
	return std::to_string(color[0]) + " " + std::to_string(color[1]) + " " + std::to_string(color[2]);
}




}