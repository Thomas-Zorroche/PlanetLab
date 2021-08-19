#include "NoiseLayer.hpp"

namespace Ceres
{

NoiseLayer::NoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& noiseSettings, std::uint32_t seed)
	: _id(id),
	_seed(seed),
	_noise(siv::PerlinNoise(seed)),
	_noiseSettings(noiseSettings)
{}

void NoiseLayer::reseed(std::uint32_t seed)
{
	_noise.reseed(seed);
	_seed = seed;
}

void NoiseLayer::reseed()
{
	_noise.reseed(_seed);
}

void NoiseLayer::setNoiseSettings(const std::shared_ptr<NoiseSettings>& noiseSettings)
{
	_noiseSettings = noiseSettings;
}

}	// ns Ceres

