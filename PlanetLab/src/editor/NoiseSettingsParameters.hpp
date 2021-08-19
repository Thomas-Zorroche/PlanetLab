#pragma once

#include "Ceres/noise/NoiseSettings.hpp"
#include "Parameter.hpp"

namespace PlanetLab
{

using ParametersMap = std::unordered_map < std::string, std::shared_ptr<ParameterBase> >;

class NoiseSettingsParameters
{
public:
	NoiseSettingsParameters(std::shared_ptr<Ceres::NoiseSettings> noiseSettings);

	void display(float sliderSpedd = 0.1f);

	void setLayerType(const Ceres::LayerType& layerType) { _layerType = layerType; }

private:
	friend const std::shared_ptr<ParameterBase>& getParameterByName(const ParametersMap& parameters, const std::string& name);

private:
	Ceres::LayerType _layerType = Ceres::LayerType::Simple;

	ParametersMap _parameters = {};

	std::vector<std::string> _parametersNames; // liste triee
};

}

