#pragma once

#include "NoiseLayer.hpp"
#include "NoiseSettings.hpp"

#include "NoiseLayer.hpp"
#include "RigidNoiseLayer.hpp"
#include "SimpleNoiseLayer.hpp"

namespace Ceres
{

/**
* @brief Factory for creating Noise Layer based on it type
*/
class NoiseLayerFactory
{
public:

	/// Create Noise Layer linked with a noise settings
	static std::shared_ptr<NoiseLayer> createNoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& settings = std::make_shared<NoiseSettings>())
	{
		switch (settings->layerType)
		{
		case LayerType::Simple:
			return std::make_shared<SimpleNoiseLayer>(id, settings);
			break;

		case LayerType::Rigid:
			return std::make_shared<RigidNoiseLayer>(id, settings);
			break;
		}
	}
};

}	// ns proceduralPlanet