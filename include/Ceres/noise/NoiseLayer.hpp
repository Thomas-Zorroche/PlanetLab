#pragma once

#include "NoiseSettings.hpp"

namespace Ceres
{

/**
* @brief Layer noise with all settings 
*/
class NoiseLayer
{
public:
	NoiseLayer(const std::shared_ptr<NoiseSettings>& settings = std::make_shared<NoiseSettings>());

	const std::shared_ptr<NoiseSettings>& getNoiseSettings() const { return _noiseSettings; }
	std::shared_ptr<NoiseSettings> getNoiseSettings() { return _noiseSettings; }

	bool isEnabled() const { return _enabled; }
	bool& isEnabled() { return _enabled; }

	bool isUsingFirstLayerAsMask() const { return _useFirstLayerAsMask; }

private:
	bool _enabled;

	bool _useFirstLayerAsMask = true;

	std::shared_ptr<NoiseSettings> _noiseSettings;
};

}