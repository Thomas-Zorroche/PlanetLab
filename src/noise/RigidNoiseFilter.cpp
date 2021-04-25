#include "RigidNoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include <algorithm>    // std::max

namespace proceduralPlanet
{

RigidNoiseFilter::RigidNoiseFilter(const std::shared_ptr<NoiseSettings>& settings, std::uint32_t seed)
	: NoiseFilter(settings, seed)
{

}

float RigidNoiseFilter::Evaluate(const glm::vec3& point) const
{
	float noiseValue = 0;

	float frequency = _settings->baseRoughness();
	float amplitude = 1;
	float weight = 1;

	for (size_t i = 0; i < _settings->layersCount(); i++)
	{
		glm::vec3 coordNoise = point * frequency + _settings->center();
		float v = 1 - abs(_noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z));
		v *= v;
		v *= weight;
		weight = std::clamp(v * _settings->weightMultiplier(), 0.0f, 1.0f);
		noiseValue += v * amplitude;

		frequency *= _settings->roughness();
		amplitude *= _settings->persistence();
	}

	noiseValue = std::max(0.0f, noiseValue - _settings->minValue());
	return noiseValue * _settings->strength();
}

}	// ns proceduralPlanet
