#pragma once

#include "noise/NoiseLayer.hpp"

namespace Ceres
{


/**
 * @brief Contains radius and noise layers (settings)
 */
class ShapeSettings
{
public:
	ShapeSettings(float radius = 1.0f);

	// Accessors

	float getPlanetRadius() const { return _planetRadius; }
	float& planetRadius() { return _planetRadius; }

	const std::vector<std::shared_ptr<NoiseLayer> >& getNoiseLayers() const { return _noiseLayers; }
	std::vector<std::shared_ptr<NoiseLayer> >& getNoiseLayers() { return _noiseLayers; }

	const std::shared_ptr<NoiseLayer>& getNoiseLayer(unsigned int index) const;
	std::shared_ptr<NoiseLayer> getNoiseLayer(unsigned int index);

	const std::shared_ptr<NoiseLayer>& getLastLayer() const { return _noiseLayers.back(); }
	std::shared_ptr<NoiseLayer> getLastLayer() { return _noiseLayers.back(); }

	// Layers

	void addLayer(const std::shared_ptr<NoiseLayer>& layer = std::make_shared<NoiseLayer>());

	void removeLastLayer();

	void removeLayer(unsigned int index);

	void removeAllLayers();

private:
	float _planetRadius;

	std::vector<std::shared_ptr<NoiseLayer> > _noiseLayers;
};

}	// ns Ceres


