#include "NoiseFilter.hpp"

NoiseFilter::NoiseFilter(int seed)
	: _noise(siv::PerlinNoise())
{

}

float NoiseFilter::evaluate(const glm::vec3& point) const
{
	return (_noise.noise3D(point.x, point.y, point.z) + 1) * 0.5;
}