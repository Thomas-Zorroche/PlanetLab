#pragma once

#include "NoiseLayer.hpp"

namespace Ceres
{

struct NoiseSettings;

/**
 * @brief Simple base noise layer
 */
class SimpleNoiseLayer : public NoiseLayer
{
public:
	SimpleNoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& settings = nullptr, std::uint32_t seed = 1);

	/**
	 * @brief Return the noise value at the given point in 3D space
	 *
	 * @param point 3D point on unit sphere
	 * @return noise value
	 */
	float evaluate(const glm::vec3& point) const override;
};

}	// ns Ceres
