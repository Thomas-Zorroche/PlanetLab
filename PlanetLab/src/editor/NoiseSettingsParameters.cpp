#include "NoiseSettingsParameters.h"

#include "ParameterFactory.hpp"

using namespace Ceres;

namespace PlanetLab
{

NoiseSettingsParameters::NoiseSettingsParameters(std::shared_ptr<Ceres::NoiseSettings> noiseSettings)
	: _filterType(noiseSettings->filterType),
	_parameters{
		{ ParameterFactory::Float("Strength",		   FilterType::Simple, ObserverFlag::MESH, noiseSettings->strength, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Base Roughness",	   FilterType::Simple, ObserverFlag::MESH, noiseSettings->baseRoughness, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Roughness",		   FilterType::Simple, ObserverFlag::MESH, noiseSettings->roughness, 0.0f, 10.0f) },
		{ ParameterFactory::Vec3("Center",			   FilterType::Simple, ObserverFlag::MESH, noiseSettings->center, 0.0f, 10.0f) },
		{ ParameterFactory::Int("Layers Count",		   FilterType::Simple, ObserverFlag::MESH, noiseSettings->layersCount, 0, 10) },
		{ ParameterFactory::Float("Persistence",	   FilterType::Simple, ObserverFlag::MESH, noiseSettings->persistence, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Min Value",		   FilterType::Simple, ObserverFlag::MESH, noiseSettings->minValue, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Weight Multiplier", FilterType::Rigid,  ObserverFlag::MESH, noiseSettings->weightMultiplier, 0.0f, 10.0f) },
	},
	// Must be sorted
	_parametersNames{
		"Strength",
		"Base Roughness",
		"Roughness",
		"Center",
		"Layers Count",
		"Persistence",
		"Min Value",
		"Weight Multiplier",
	}
{};

// Friend function
const std::shared_ptr<PlanetLab::ParameterBase>& getParameterByName(const PlanetLab::ParametersMap& parameters, const std::string& name)
{
	auto it = parameters.find(name);
	if (it == parameters.end())
	{
		std::cout << "ERROR: cannot find parameter name " << name << std::endl;
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
		if (parameter->GetType() == _filterType || parameter->GetType() == Ceres::FilterType::Simple)
			parameter->Display(sliderSpeed);
	}
}




}