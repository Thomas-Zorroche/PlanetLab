#pragma once

#include "PerlinNoise.hpp"
#include "glm/glm.hpp"

class NoiseFilter
{
public:
	NoiseFilter(int seed = 1);

	float evaluate(const glm::vec3& point) const;

private:
	siv::PerlinNoise _noise;
};
