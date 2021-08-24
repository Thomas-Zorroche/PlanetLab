#pragma once

#include "Ceres/noise/NoiseLayer.hpp"
#include "Ceres/Planet.hpp"
#include "editor/NoiseSettingsParameters.hpp"


namespace PlanetLab
{

class UINoiseLayer
{
public:
	UINoiseLayer(std::shared_ptr<Ceres::NoiseLayer> noiseLayer, 
		std::shared_ptr<Ceres::Planet> planet, float& sliderSpeed);

	bool draw(unsigned int selectedLayerId);


private:
	std::shared_ptr<Ceres::NoiseLayer> _noiseLayer;

	NoiseSettingsParameters _noiseSettingsParameters;

	std::string _defaultName = "";

	unsigned int _id = 0;

	std::string _name = "Noise Layer";

	bool _locked = false;

	std::shared_ptr<Ceres::Planet> _planet;

	float& _sliderSpeed;

	bool _open = true;
};


}