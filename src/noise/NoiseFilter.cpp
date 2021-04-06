#include "NoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include <algorithm>    // std::max


NoiseFilter::NoiseFilter(const std::shared_ptr<NoiseSettings>& settings, int seed)
	: _noise(siv::PerlinNoise()), _settings(settings)
{

}

float NoiseFilter::evaluate(const glm::vec3& point) const
{
	//float noiseValue = (_noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z) + 1) * 0.5;
	float noiseValue = 0;

	float frequency = _settings->baseRoughness();
	float amplitude = 1;

	for (size_t i = 0; i < _settings->layersCount(); i++)
	{
		glm::vec3 coordNoise = point * frequency + _settings->center();
		float v = _noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z);
		noiseValue += (v + 1) * 0.5f * amplitude;

		frequency *= _settings->roughness();
		amplitude *= _settings->persistence();
	}

	noiseValue = std::max(0.0f, noiseValue - _settings->minValue());
	return noiseValue * _settings->strength();
}

void NoiseFilter::setSettings(const std::shared_ptr<NoiseSettings>& settings)
{
	_settings = settings;
}