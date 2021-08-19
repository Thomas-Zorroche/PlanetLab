#pragma once

#include "Ceres/Planet.hpp"
#include "editor/UINoiseLayer.hpp"

namespace PlanetLab
{


class SettingsPanel
{
public:
	SettingsPanel(std::shared_ptr<Ceres::Planet> planet = nullptr);

	void draw();

	void setSliderSpeed(float speed) { _sliderSpeed = speed; }

	void setPlanet(std::shared_ptr<Ceres::Planet> planet) { _planet = planet; }

	void clearNoiseLayers();
	void addNoiseLayer(std::shared_ptr<Ceres::NoiseLayer> noiselayer);

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

	std::vector<UINoiseLayer> _uiNoiseLayers;

	float _sliderSpeed = 0.08;
};

}