#include "noise/NoiseSettings.hpp"
#include "editor/Parameter.hpp"
#include "editor/ParameterFactory.hpp"

NoiseSettings::NoiseSettings()
	: _parameters {
		{ ParameterFactory::Float("SSStrength", FilterType::Simple, ObserverFlag::NOISE, 1.0f, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Base Roughness", FilterType::Simple, ObserverFlag::NOISE, 1.0f, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Roughness", FilterType::Simple, ObserverFlag::NOISE, 1.0f, 0.0f, 10.0f) },
		{ ParameterFactory::Vec3("Center", FilterType::Simple, ObserverFlag::NOISE, glm::vec3(0), 0.0f, 10.0f) },
		{ ParameterFactory::Int("Layers Count", FilterType::Simple, ObserverFlag::NOISE, 1, 0, 10) },
		{ ParameterFactory::Float("Persistence", FilterType::Simple, ObserverFlag::NOISE, .5f, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Min Value", FilterType::Simple, ObserverFlag::NOISE, 0.0f, 0.0f, 10.0f) },
		{ ParameterFactory::Float("Weight Multiplier", FilterType::Rigid, ObserverFlag::NOISE, 0.5f, 0.0f, 10.0f) },
	},
	_parametersNames{
		"SSStrength",
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

float NoiseSettings::strength() const { return Float(_parameters, "SSStrength"); }
float& NoiseSettings::strength() { return FloatPtr(_parameters, "SSStrength"); }

float NoiseSettings::baseRoughness() const { return Float(_parameters, "Base Roughness"); }
float& NoiseSettings::baseRoughness() { return FloatPtr(_parameters, "Base Roughness"); }

float NoiseSettings::roughness() const { return Float(_parameters, "Roughness"); }
float& NoiseSettings::roughness() { return FloatPtr(_parameters, "Roughness"); }

glm::vec3 NoiseSettings::center() const { return Vec3(_parameters, "Center"); }
glm::vec3& NoiseSettings::center() { return Vec3Ptr(_parameters, "Center"); }

int NoiseSettings::layersCount() const { return Int(_parameters, "Layers Count"); }
int& NoiseSettings::layersCount() { return IntPtr(_parameters, "Layers Count"); }

float NoiseSettings::persistence() const { return Float(_parameters, "Persistence"); }
float& NoiseSettings::persistence() { return FloatPtr(_parameters, "Persistence"); }

float NoiseSettings::minValue() const { return Float(_parameters, "Min Value"); }
float& NoiseSettings::minValue() { return FloatPtr(_parameters, "Min Value"); }

float NoiseSettings::weightMultiplier() const { return Float(_parameters, "Weight Multiplier"); }
float& NoiseSettings::weightMultiplier() { return FloatPtr(_parameters, "Weight Multiplier"); }


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
std::shared_ptr<ParameterBase>& GetParameterPtrByName(ParametersMap& parameters, const std::string& name)
{
	auto it = parameters.find(name);
	if (it == parameters.end())
	{
		std::cout << "ERROR: cannot find parameter ptr name " << name << std::endl;
	}	
	return it->second;
}

float Float(const ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterFloat>(parameterBase))
		return parameter->GetValue();
}
float& FloatPtr(ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterPtrByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterFloat>(parameterBase))
		return parameter->GetValue();
}

int Int(const ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterInt>(parameterBase))
		return parameter->GetValue();
}
int& IntPtr(ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterPtrByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterInt>(parameterBase))
		return parameter->GetValue();
}

glm::vec3 Vec3(const ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterVec3>(parameterBase))
		return parameter->GetValue();
}
glm::vec3& Vec3Ptr(ParametersMap& parameters, const std::string& name)
{
	auto parameterBase = GetParameterPtrByName(parameters, name);
	if (auto parameter = std::dynamic_pointer_cast<ParameterVec3>(parameterBase))
		return parameter->GetValue();
}

