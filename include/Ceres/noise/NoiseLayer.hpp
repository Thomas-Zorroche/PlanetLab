#pragma once

#include "PerlinNoise.hpp"
#include "NoiseSettings.hpp"

namespace Ceres
{


/**
 * @brief Noise layer abstract class.
 * Contains the noise algorithm (Here Perlin Noise)
 */
class NoiseLayer
{
public:
	NoiseLayer(unsigned int id, const std::shared_ptr<NoiseSettings>& noiseSettings = nullptr, std::uint32_t seed = 1);

	virtual ~NoiseLayer() {};

	virtual std::uint32_t& getSeed() { return _seed; }

	virtual void setNoiseSettings(const std::shared_ptr<NoiseSettings>& settings);

	const std::shared_ptr<NoiseSettings>& getNoiseSettings() const { return _noiseSettings; }
	std::shared_ptr<NoiseSettings> getNoiseSettings() { return _noiseSettings; }

	/**
	 * @brief Return the noise value at the given point in 3D space
	 * 
	 * @param point 3D point on unit sphere
	 * @return noise value
	 */
	virtual float evaluate(const glm::vec3& point) const = 0;

	virtual void reseed(std::uint32_t seed);

	virtual void reseed();

	int getId() const { return _id; }
	
	bool isEnabled() const { return _enabled; }
	bool& isEnabled() { return _enabled; }

	bool isInverted() const { return _invert; }
	bool& isInverted() { return _invert; }

	bool firstLayerUsedAsMask() const { return _useFirstLayerAsMask; }
	bool& firstLayerUsedAsMask() { return _useFirstLayerAsMask; }


protected:
	/// Seed for randomness noise
	std::uint32_t _seed;

	/// Perlin noise algorithm
	siv::PerlinNoise _noise;

	int _id = -1;
	
	bool _enabled = true;

	bool _invert = false;

	bool _useFirstLayerAsMask = true;

	/// Noise Settings of the noise layer linked with this noise layer
	std::shared_ptr<NoiseSettings> _noiseSettings;
};

}	// ns Ceres