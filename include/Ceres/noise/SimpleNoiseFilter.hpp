#pragma once

#include "NoiseFilter.hpp"

namespace Ceres
{

struct NoiseSettings;

/**
 * @brief Simple base noise filter
 */
class SimpleNoiseFilter : public NoiseFilter
{
public:
	SimpleNoiseFilter(const std::shared_ptr<NoiseSettings>& settings = nullptr, std::uint32_t seed = 1);

	/**
	 * @brief Return the noise value at the given point in 3D space
	 *
	 * @param point 3D point on unit sphere
	 * @return noise value
	 */
	float evaluate(const glm::vec3& point) const override;
};

}	// ns Ceres
