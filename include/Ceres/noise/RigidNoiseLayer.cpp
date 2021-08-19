#include "RigidNoiseLayer.hpp"
#include "NoiseSettings.hpp"

namespace Ceres
{

RigidNoiseLayer::RigidNoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& settings, std::uint32_t seed)
	: NoiseLayer(id, settings, seed)
{}

float RigidNoiseLayer::evaluate(const glm::vec3& point) const
{
	float noiseValue = 0;

	float frequency = _noiseSettings->baseRoughness;
	float amplitude = 1;
	float weight = 1;

	for (size_t i = 0; i < _noiseSettings->iterations; i++)
	{
		glm::vec3 coordNoise = point * frequency + _noiseSettings->center;
		float v = 1 - abs(_noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z));
		v *= v;
		v *= weight;
		weight = std::clamp(v * _noiseSettings->weightMultiplier, 0.0f, 1.0f);
		noiseValue += v * amplitude;

		frequency *= _noiseSettings->roughness;
		amplitude *= _noiseSettings->persistence;
	}

	noiseValue = std::max(0.0f, noiseValue - _noiseSettings->minValue);
	noiseValue *= _noiseSettings->strength;

	if (_invert)
		noiseValue *= -1;

	return noiseValue;
}

}	// ns Ceres
