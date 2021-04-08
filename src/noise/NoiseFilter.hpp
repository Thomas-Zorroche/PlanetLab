#pragma once

#include "PerlinNoise.hpp"
#include "glm/glm.hpp"

#include <memory>

class NoiseSettings;

class NoiseFilter
{
public:
	NoiseFilter(const std::shared_ptr<NoiseSettings>& settings = nullptr,
		std::uint32_t seed = 1);

	float evaluate(const glm::vec3& point) const;

	void setSettings(const std::shared_ptr<NoiseSettings>& settings);

	void reseed(std::uint32_t seed);
	void reseed();

	std::uint32_t& seed() { return _seed; }

private:
	std::uint32_t _seed;
	siv::PerlinNoise _noise;
	std::shared_ptr<NoiseSettings> _settings;
};
