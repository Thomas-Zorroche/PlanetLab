#pragma once

#include "NoiseFilter.hpp"
#include <memory>

namespace Ceres
{

class NoiseSettings;

class SimpleNoiseFilter : public NoiseFilter
{
public:
	SimpleNoiseFilter(
		const std::shared_ptr<NoiseSettings>& settings = nullptr,
		std::uint32_t seed = 1
	);

	float Evaluate(const glm::vec3& point) const override;
};

}	// ns proceduralPlanet
