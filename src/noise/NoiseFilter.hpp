#pragma once

#include "PerlinNoise.hpp"
#include "glm/glm.hpp"

#include <memory>

class NoiseSettings;

class NoiseFilter
{
public:
	NoiseFilter(const std::shared_ptr<NoiseSettings>& settings, int seed = 1);

	float evaluate(const glm::vec3& point) const;

private:
	siv::PerlinNoise _noise;
	std::shared_ptr<NoiseSettings> _settings;

};
