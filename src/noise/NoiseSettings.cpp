#include "noise/NoiseSettings.hpp"
#include "editor/Parameter.hpp"
#include "editor/ParameterFactory.hpp"

namespace proceduralPlanet
{

NoiseSettings::NoiseSettings()
	: _parameters {
		{ ParameterFactory::Float("Strength", FilterType::Simple, editor::ObserverFlag::MESH, _strength, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Base Roughness", FilterType::Simple, editor::ObserverFlag::MESH, _baseRoughness, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Roughness", FilterType::Simple, editor::ObserverFlag::MESH, _roughness, 0.0f, 10.0f) },
		{ ParameterFactory::Vec3("Center", FilterType::Simple, editor::ObserverFlag::MESH, _center, 0.0f, 10.0f) },
		{ ParameterFactory::Int("Layers Count", FilterType::Simple, editor::ObserverFlag::MESH, _layersCount, 0, 10) },
		{ ParameterFactory::Float("Persistence", FilterType::Simple, editor::ObserverFlag::MESH, _persistence, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Min Value", FilterType::Simple, editor::ObserverFlag::MESH, _minValue, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Weight Multiplier", FilterType::Rigid, editor::ObserverFlag::MESH, _weightMultiplier, 0.0f, 10.0f) },
	},
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
{
};

void NoiseSettings::Display()
{
	for (const auto& name : _parametersNames)
	{
		auto parameter = GetParameterByName(_parameters, name);
		if (parameter->GetType() == _filterType || parameter->GetType() == FilterType::Simple)
			parameter->Display();
	}
}

FilterType& NoiseSettings::GetFilterType()
{ 
	return _filterType; 
}

float NoiseSettings::strength() const { return _strength; }
float& NoiseSettings::strength() { return _strength; }

float NoiseSettings::baseRoughness() const { return _baseRoughness; }
float& NoiseSettings::baseRoughness() { return _baseRoughness; }

float NoiseSettings::roughness() const { return _roughness; }
float& NoiseSettings::roughness() { return _roughness; }

glm::vec3 NoiseSettings::center() const { return _center; }
glm::vec3& NoiseSettings::center() { return _center; }

int NoiseSettings::layersCount() const { return _layersCount; }
int& NoiseSettings::layersCount() { return _layersCount; }

float NoiseSettings::persistence() const { return _persistence; }
float& NoiseSettings::persistence() { return _persistence; }

float NoiseSettings::minValue() const { return _minValue; }
float& NoiseSettings::minValue() { return _minValue; }

float NoiseSettings::weightMultiplier() const { return _weightMultiplier; }
float& NoiseSettings::weightMultiplier() { return _weightMultiplier; }


/*
* Friends Functions
*/
const std::shared_ptr<ParameterBase>& GetParameterByName(const ParametersMap& parameters, const std::string& name)
{
	auto it = parameters.find(name);
	if (it == parameters.end())
	{
		std::cout << "ERROR: cannot find parameter name " << name << std::endl;
		return nullptr;
	}
	else
		return it->second;
}

}	// ns proceduralPlanet

