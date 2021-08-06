#include "SimpleNoiseFilter.hpp"
#include "NoiseSettings.hpp"

namespace Ceres
{

SimpleNoiseFilter::SimpleNoiseFilter(const std::shared_ptr<NoiseSettings>& settings, std::uint32_t seed)
	: NoiseFilter(settings, seed)
{}

float SimpleNoiseFilter::evaluate(const glm::vec3& point) const
{
	float noiseValue = 0;

	float frequency = _settings->baseRoughness;
	float amplitude = 1;

	for (size_t i = 0; i < _settings->layersCount; i++)
	{
		glm::vec3 coordNoise = point * frequency + _settings->center;
		float v = _noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z);
		noiseValue += (v + 1) * 0.5f * amplitude;

		frequency *= _settings->roughness;
		amplitude *= _settings->persistence;
	}

	noiseValue = noiseValue - _settings->minValue;
	return noiseValue * _settings->strength;
}

}	// ns Ceres