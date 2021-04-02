#include "NoiseFilter.hpp"

#include "NoiseSettings.hpp"


NoiseFilter::NoiseFilter(const std::shared_ptr<NoiseSettings>& settings, int seed)
	: _noise(siv::PerlinNoise()), _settings(settings)
{

}

float NoiseFilter::evaluate(const glm::vec3& point) const
{
	glm::vec3 coordNoise = point * _settings->roughness() + _settings->center();
	float noiseValue = (_noise.noise3D(coordNoise.x, coordNoise.y, coordNoise.z) + 1) * 0.5;
	return noiseValue * _settings->strength();
}