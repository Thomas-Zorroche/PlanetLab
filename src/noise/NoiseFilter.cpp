#include "NoiseFilter.hpp"

#include "NoiseSettings.hpp"


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

	for (size_t i = 0; i < _settings->layerCount(); i++)
	{
		glm::vec3 coordNoise = point * frequency + _settings->center();
		float v = _noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z);
		noiseValue += (v + 1) * 0.5f * amplitude;

		frequency *= _settings->roughness();
		amplitude *= _settings->persistence();
	}
	
	return noiseValue * _settings->strength();
}