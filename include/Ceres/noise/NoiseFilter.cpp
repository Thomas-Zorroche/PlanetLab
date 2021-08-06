#include "NoiseFilter.hpp"

namespace Ceres
{

NoiseFilter::NoiseFilter(const std::shared_ptr<NoiseSettings>& settings, std::uint32_t seed)
	: _seed(seed),
	_noise(siv::PerlinNoise(seed)),
	_settings(settings)
{}

void NoiseFilter::reseed(std::uint32_t seed)
{
	_noise.reseed(seed);
	_seed = seed;
}

void NoiseFilter::reseed()
{
	_noise.reseed(_seed);
}

void NoiseFilter::setSettings(const std::shared_ptr<NoiseSettings>& settings)
{
	_settings = settings;
}

}	// ns Ceres

