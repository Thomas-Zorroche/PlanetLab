#pragma once


namespace Ceres
{

class NoiseFilter;
class ShapeSettings;

/**
 * @brief Responsible to apply noises algorithms on the surface mesh
 */
class ShapeGenerator
{
public:
	ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings);

	// Filters

	std::vector<std::shared_ptr<NoiseFilter> > getNoiseFilters();

	std::shared_ptr<NoiseFilter> getNoiseFilter(unsigned int index) const;

	void updateFilterType(std::uint32_t index);

	void addFilter(const std::shared_ptr<NoiseFilter>& layer);

	void removeLastFilter();

	void removeFilter(unsigned int index);

	void removeAllFilters();


	// Elevation

	/**
	 * @brief Compute elevation of the given point after noise deformation
	 * 
	 * @param pointOnUnitSphere point on the unit normalize sphere
	 * @return elevation value (positive or negative)
	 */
	float calculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const;

	/**
	 * @brief Compute elevation from the surface of the water
	 *
	 * @param unscaledElevation elevation from noise
	 * @return elevation from water (only positive)
	 */
	float getScaledElevation(float unscaledElevation) const;

private:
	std::shared_ptr<ShapeSettings> _settings;

	std::vector<std::shared_ptr<NoiseFilter> > _noiseFilters;
};

}	// ns proceduralPlanet