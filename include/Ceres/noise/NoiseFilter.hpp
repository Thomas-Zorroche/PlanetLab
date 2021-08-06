#pragma once

#include "PerlinNoise.hpp"
#include "NoiseSettings.hpp"

namespace Ceres
{


/**
 * @brief Noise filter abstract class.
 * Contains the noise algorithm (Here Perlin Noise)
 */
class NoiseFilter
{
public:
	NoiseFilter(const std::shared_ptr<NoiseSettings>& settings = nullptr, std::uint32_t seed = 1);

	virtual ~NoiseFilter() {};

	virtual std::uint32_t& getSeed() { return _seed; }

	virtual void setSettings(const std::shared_ptr<NoiseSettings>& settings);

	/**
	 * @brief Return the noise value at the given point in 3D space
	 * 
	 * @param point 3D point on unit sphere
	 * @return noise value
	 */
	virtual float evaluate(const glm::vec3& point) const = 0;

	virtual void reseed(std::uint32_t seed);

	virtual void reseed();


protected:
	/// Seed for randomness noise
	std::uint32_t _seed;

	/// Perlin noise algorithm
	siv::PerlinNoise _noise;

	/// Noise Settings of the noise layer linked with this noise filter
	std::shared_ptr<NoiseSettings> _settings;
};

}	// ns Ceres