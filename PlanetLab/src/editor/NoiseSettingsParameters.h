#pragma once

#include "Ceres/noise/NoiseSettings.hpp"
#include "Parameter.hpp"

namespace PlanetLab
{

using ParametersMap = std::unordered_map < std::string, std::shared_ptr<ParameterBase> >;

class NoiseSettingsParameters
{
public:
	NoiseSettingsParameters(std::shared_ptr<Ceres::NoiseSettings>& noiseSettings);

	void display();

	void setFilterType(const Ceres::FilterType& filterType) { _filterType = filterType; }

private:
	friend const std::shared_ptr<ParameterBase>& getParameterByName(const ParametersMap& parameters, const std::string& name);

private:
	Ceres::FilterType _filterType = Ceres::FilterType::Simple;

	ParametersMap _parameters;

	std::vector<std::string> _parametersNames; // liste triée
};

}

