#include "NoiseFilter.hpp"

namespace proceduralPlanet
{

NoiseFilter::NoiseFilter(
	const std::shared_ptr<NoiseSettings>& settings,
	std::uint32_t seed
)
	: _seed(seed),
	_noise(siv::PerlinNoise(seed)),
	_settings(settings)
{

}

void NoiseFilter::Reseed(std::uint32_t seed)
{
	_noise.reseed(seed);
	_seed = seed;
}

void NoiseFilter::Reseed()
{
	_noise.reseed(_seed);
}

std::uint32_t& NoiseFilter::Seed()
{
	return _seed;
}

void NoiseFilter::SetSettings(const std::shared_ptr<NoiseSettings>& settings)
{
	_settings = settings;
}

}	// ns proceduralPlanet

