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

	void draw();


private:
	std::shared_ptr<Ceres::NoiseLayer> _noiseLayer;

	NoiseSettingsParameters _noiseSettingsParameters;

	std::string _name = "";

	unsigned int _id = 0;

	bool _locked = false;

	std::shared_ptr<Ceres::Planet> _planet;

	float& _sliderSpeed;
};


}