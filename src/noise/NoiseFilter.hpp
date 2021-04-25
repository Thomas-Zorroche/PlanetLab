#pragma once

#include "glm/glm.hpp"
#include <memory>

#include "PerlinNoise.hpp"
#include "NoiseSettings.hpp"

namespace proceduralPlanet
{

/*
* Abstact Class For all the noise filters
*/
class NoiseFilter
{
public:
	NoiseFilter(
		const std::shared_ptr<NoiseSettings>& settings = nullptr,
		std::uint32_t seed = 1
	);

	virtual ~NoiseFilter() {};

	virtual float Evaluate(const glm::vec3& point) const = 0;

	virtual void Reseed(std::uint32_t seed);

	virtual void Reseed();

	virtual std::uint32_t& Seed();

	virtual void SetSettings(const std::shared_ptr<NoiseSettings>& settings);

protected:
	std::uint32_t _seed;
	siv::PerlinNoise _noise;
	std::shared_ptr<NoiseSettings> _settings;
};

}	// ns proceduralPlanet