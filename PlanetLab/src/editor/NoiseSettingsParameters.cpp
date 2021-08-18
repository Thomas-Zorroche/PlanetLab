#include "NoiseSettingsParameters.h"
#include "ParameterFactory.hpp"

using namespace Ceres;

namespace PlanetLab
{

NoiseSettingsParameters::NoiseSettingsParameters(std::shared_ptr<Ceres::NoiseSettings> noiseSettings)
	: _filterType(noiseSettings->filterType),
	// Must be sorted
	_parametersNames{
		"Strength",
		"Base Roughness",
		"Roughness",
		"Center",
		"Iterations",
		"Persistence",
		"Min Value",
		"Weight Multiplier",
	}
{
	_parameters.insert(ParameterFactory::Float("Strength", FilterType::Simple, noiseSettings->strength, 0.0f, 5.0f,
		"Strength of this noise layer."));
	_parameters.insert(ParameterFactory::Float("Base Roughness", FilterType::Simple, noiseSettings->baseRoughness, 0.0f, 10.0f,
		"Initial frequency of noise texture."));
	_parameters.insert(ParameterFactory::Float("Roughness", FilterType::Simple, noiseSettings->roughness, 0.0f, 10.0f,
		"This will multiply frequency with each layer."));
	_parameters.insert(ParameterFactory::Vec3("Center", FilterType::Simple, noiseSettings->center, 0.0f, 10.0f,
		"Center of noise texture."));
	_parameters.insert(ParameterFactory::Int("Iterations", FilterType::Simple, noiseSettings->iterations, 0.0f, 10,
		"Iterations of noise filters apply to this layer."));
	_parameters.insert(ParameterFactory::Float("Persistence", FilterType::Simple, noiseSettings->persistence, 0.0f, 10.0f,
		"This will multiply amplitude with each layer."));
	_parameters.insert(ParameterFactory::Float("Min Value", FilterType::Simple, noiseSettings->minValue, 0.0f, 5.0f,
		"Decrease noise value."));
	_parameters.insert(ParameterFactory::Float("Weight Multiplier", FilterType::Rigid, noiseSettings->weightMultiplier, 0.0f, 5.0f,
		"This will multiply noise value with previous iterations values.\nFor regions with high elevations, value will be increased."));
};

// Friend function
const std::shared_ptr<PlanetLab::ParameterBase>& getParameterByName(const PlanetLab::ParametersMap& parameters, const std::string& name)
{
	auto it = parameters.find(name);
	if (it == parameters.end())
	{
		PLANETLAB_ERROR("ERROR: cannot find parameter name: {}", name);
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

void NoiseSettingsParameters::display(float sliderSpeed)
{
	for (const auto& name : _parametersNames)
	{
		auto parameter = getParameterByName(_parameters, name);
		if (parameter && (parameter->GetType() == _filterType || parameter->GetType() == Ceres::FilterType::Simple))
			parameter->Display(sliderSpeed);
	}
}




}