#pragma once

#include <memory>
#include <vector>

#include "noise/NoiseSettings.hpp"

namespace Ceres
{

class NoiseLayer
{
public:
	NoiseLayer(const std::shared_ptr<NoiseSettings>& settings = std::make_shared<NoiseSettings>());

	std::shared_ptr<NoiseSettings>& noiseSettings() { return _noiseSettings; }
	bool enabled() const { return _enabled; }
	bool& enabled() { return _enabled; }
	bool useFirstLayerAsMask() const { return _useFirstLayerAsMask; }

private:
	bool _enabled;
	bool _useFirstLayerAsMask = true;
	std::shared_ptr<NoiseSettings> _noiseSettings;
};

class ShapeSettings
{
public:
	ShapeSettings(float radius = 1.0f);
	float& planetRadius();
	float planetRadius() const;

	std::vector<std::shared_ptr<NoiseLayer> > noiseLayers();
	std::shared_ptr<NoiseLayer> noiseLayer(unsigned int index);

	void addLayer(const std::shared_ptr<NoiseLayer>& layer);
	void removeLastLayer();
	void removeLayer(unsigned int index);
	void removeAllLayers();

private:
	float _planetRadius;
	std::vector<std::shared_ptr<NoiseLayer> > _noiseLayers;
};

}	// ns proceduralPlanet


