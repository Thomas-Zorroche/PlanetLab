#include "SimpleNoiseLayer.hpp"
#include "NoiseSettings.hpp"

namespace Ceres
{

SimpleNoiseLayer::SimpleNoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& settings, std::uint32_t seed)
	: NoiseLayer(id, settings, seed)
{}

float SimpleNoiseLayer::evaluate(const glm::vec3& point) const
{
	float noiseValue = 0;

	float frequency = _noiseSettings->baseRoughness;
	float amplitude = 1;

	for (size_t i = 0; i < _noiseSettings->iterations; i++)
	{
		glm::vec3 coordNoise = point * frequency + _noiseSettings->center;
		float v = _noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z);
		noiseValue += (v + 1) * 0.5f * amplitude;

		frequency *= _noiseSettings->roughness;
		amplitude *= _noiseSettings->persistence;
	}

	noiseValue = (noiseValue - _noiseSettings->minValue) * _noiseSettings->strength;

	if (_invert)
		noiseValue *= -1;

	return noiseValue;
}

}	// ns Ceres