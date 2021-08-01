#pragma once

#include "NoiseFilter.hpp"

namespace Ceres
{

class NoiseSettings;

class RigidNoiseFilter : public NoiseFilter
{
public:
	RigidNoiseFilter(
		const std::shared_ptr<NoiseSettings>& settings = nullptr,
		std::uint32_t seed = 1
	);

	float Evaluate(const glm::vec3& point) const override;
};

}	// ns proceduralPlanet