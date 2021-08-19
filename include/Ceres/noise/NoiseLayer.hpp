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
	NoiseLayer(int id = -1, const std::shared_ptr<NoiseSettings>& settings = std::make_shared<NoiseSettings>());

	const std::shared_ptr<NoiseSettings>& getNoiseSettings() const { return _noiseSettings; }
	std::shared_ptr<NoiseSettings> getNoiseSettings() { return _noiseSettings; }

	bool isEnabled() const { return _enabled; }
	bool& isEnabled() { return _enabled; }

	bool isUsingFirstLayerAsMask() const { return _useFirstLayerAsMask; }

	int getId() const { return _id; }

private:
	bool _enabled;

	bool _useFirstLayerAsMask = true;

	std::shared_ptr<NoiseSettings> _noiseSettings;

	int _id = -1;
};

}