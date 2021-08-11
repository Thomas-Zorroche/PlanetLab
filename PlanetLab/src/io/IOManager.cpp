#include "IOManager.hpp"
#include "ceres/Planet.hpp"
#include "ceres/ShapeGenerator.hpp"
#include "ceres/noise/NoiseFilter.hpp"

namespace PlanetLab
{

using namespace Ceres;

void IOManager::newFile()
{
	_fileOnDisk = false;
	_currentFileName = "New Scene";
	_fileValuesSaved = false;
	updateTitleWindow();
}


bool IOManager::save(std::shared_ptr<Planet>& planet)
{
	if (_fileOnDisk && !_fileValuesSaved)
	{
		return saveAs(_currentFileName, planet);
	}
	return false;
}

bool IOManager::saveAs(const std::string& outputFileName, std::shared_ptr<Planet>& planet)
{
	// create a file instance
	mINI::INIFile file(outputFileName);

	// create a data structure
	mINI::INIStructure ini;

	// populate the structure
	ini["layers"]["count"] = std::to_string(planet->getShapeSettings()->getNoiseLayers().size());

	int layerCount = 1;
	for (const auto& layer : planet->getShapeSettings()->getNoiseLayers())
	{
		std::string section = "noiseSettings_" + std::to_string(layerCount);

		std::string type = (layer->getNoiseSettings()->filterType == FilterType::Simple) ? "simple" : "rigid"; // TODO for the moment, there are just 2 type of noise...
		ini[section]["type"] = type;
		
		ini[section]["seed"] = std::to_string(planet->getShapeGenerator()->getNoiseFilter(layerCount - 1)->getSeed());

		ini[section]["strength"] = std::to_string(layer->getNoiseSettings()->strength);
		ini[section]["layersCount"] = std::to_string(layer->getNoiseSettings()->layersCount);
		ini[section]["baseRoughness"] = std::to_string(layer->getNoiseSettings()->baseRoughness);
		ini[section]["roughness"] = std::to_string(layer->getNoiseSettings()->roughness);
		ini[section]["persistence"] = std::to_string(layer->getNoiseSettings()->persistence);
		ini[section]["centerX"] = std::to_string(layer->getNoiseSettings()->center.x);
		ini[section]["centerY"] = std::to_string(layer->getNoiseSettings()->center.y);
		ini[section]["centerZ"] = std::to_string(layer->getNoiseSettings()->center.z);
		ini[section]["minValue"] = std::to_string(layer->getNoiseSettings()->minValue);
		ini[section]["weightMultiplier"] = std::to_string(layer->getNoiseSettings()->weightMultiplier);

		layerCount++;
	}

	// Save color settings
	ini["colorSettings"]["useOceanColor"] = std::to_string(planet->getColorSettings()->getUseOceanColor());
	ini["colorSettings"]["oceanDepth"] = std::to_string(planet->getColorSettings()->getOceanDepth());
	ini["colorSettings"]["oceanColor"] = writeColor(planet->getColorSettings()->getOceanColor());
	ini["colorSettings"]["planetColor"] = writeColor(planet->getColorSettings()->getLandmassColor());
	ini["colorSettings"]["useLandmassRamp"] = std::to_string(planet->getColorSettings()->getUseLandmassRamp());
	
	// Save Gradient Settings
	const auto& marks = planet->getColorSettings()->getGradient().getMarks();
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
		_fileOnDisk = true;
		_currentFileName = outputFileName;
		_fileValuesSaved = true;
		updateTitleWindow();
		return true;
	}
	else
	{
		_fileOnDisk = false;
		_fileValuesSaved = false;
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

	_fileOnDisk = true;
	_currentFileName = inputFileName;
	_fileValuesSaved = true;
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
	planet->reset();

	std::string& layersCountStr = ini.get("layers").get("count");
	const int layersCount = std::atoi(layersCountStr.c_str());

	planet->addNoiseLayer(layersCount);

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
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->filterType = (typeStr == "simple") ? FilterType::Simple : FilterType::Rigid;
		planet->getShapeGenerator()->updateFilterType(i - 1);
		planet->getShapeGenerator()->getNoiseFilter(i - 1)->reseed(std::atof(seedStr.c_str()));
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->strength = std::atof(strengthStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->layersCount = std::atof(layersCountStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->baseRoughness = std::atof(baseRoughnessStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->roughness = std::atof(roughnessStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->center.x = std::atof(centerXStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->center.y = std::atof(centerYStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->center.z = std::atof(centerZStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->minValue = std::atof(minValueStr.c_str());
		planet->getShapeSettings()->getNoiseLayer(i - 1)->getNoiseSettings()->weightMultiplier = std::atof(weightMultiplierStr.c_str());
	}

	// Load Color values
	std::string& useoceancolorStr = ini.get("colorsettings").get("useoceancolor");
	std::string& oceandepthStr = ini.get("colorsettings").get("oceandepth");
	std::string& oceancolorStr = ini.get("colorsettings").get("oceancolor");
	std::string& planetcolorStr = ini.get("colorsettings").get("planetcolor");
	std::string& uselandmassrampStr = ini.get("colorsettings").get("uselandmassramp");

	planet->getColorSettings()->getUseOceanColor() = std::atoi(useoceancolorStr.c_str());
	planet->getColorSettings()->getOceanDepth() = std::atoi(oceandepthStr.c_str());
	planet->getColorSettings()->getOceanColor() = parseColor(oceancolorStr);
	planet->getColorSettings()->getLandmassColor() = parseColor(planetcolorStr);
	planet->getColorSettings()->getUseLandmassRamp() = std::atoi(oceandepthStr.c_str());

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
	planet->getColorSettings()->setColorMarks(marks);

	planet->update(ObserverFlag::MESH);
	planet->update(ObserverFlag::COLOR);
}

void IOManager::updateTitleWindow()
{
	if (areUnsavedValues())
	{
		Application::Get().updateTitle(std::string("PlanetLab * " + _currentFileName));
	}
	else
	{
		Application::Get().updateTitle(std::string("PlanetLab " + _currentFileName));
	}
}

Ceres::Color IOManager::parseColor(const std::string& vec3String)
{
	if (vec3String == "") return Ceres::Color();

	std::vector<float> color;
	std::string value;
	std::istringstream stream(vec3String);
	while (std::getline(stream, value, ' '))
	{
		color.push_back(std::atof(value.c_str()));
	}

	return Ceres::Color(color[0], color[1], color[2]);
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