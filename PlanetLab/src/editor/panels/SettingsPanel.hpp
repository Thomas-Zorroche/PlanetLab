#pragma once

#include "Ceres/Planet.hpp"
#include "editor/NoiseSettingsParameters.h"

namespace PlanetLab
{


class SettingsPanel
{
public:
	SettingsPanel(std::shared_ptr<Ceres::Planet> planet = nullptr);

	void draw();

	void setSliderSpeed(float speed) { _sliderSpeed = speed; }

	void setPlanet(std::shared_ptr<Ceres::Planet> planet) { _planet = planet; }

	void clearNoiseSettings() { _noiseSettingsParameters.clear(); }
	void addNoiseSettings(NoiseSettingsParameters noiseSettings) { _noiseSettingsParameters.push_back(noiseSettings); }

private:
	void displayPlanetPanel();
	void displayNoisePanel();
	void displayMaterialPanel();
	void displayWorldPanel();
	void displayOverlaysPanel();

	void drawUpdateModeItem();
	void updateNoiseLayersCount(int noiseLayersCountUpdated);

private:
	std::shared_ptr<Ceres::Planet> _planet;

	std::vector<NoiseSettingsParameters> _noiseSettingsParameters;

	float _sliderSpeed = 0.08;
};

}