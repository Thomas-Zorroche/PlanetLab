#pragma once

#include "glm/glm.hpp"

#include <memory>
#include <vector>


namespace Ceres
{

class NoiseFilter;
class ShapeSettings;

class ShapeGenerator
{
public:
	ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings);

	float calculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const;
	float ShapeGenerator::getScaledElevation(float unscaledElevation) const;

	std::vector<std::shared_ptr<NoiseFilter> > noiseFilters();
	std::shared_ptr<NoiseFilter> noiseFilter(unsigned int index) const;

	void addFilter(const std::shared_ptr<NoiseFilter>& layer);
	void removeLastFilter();
	void removeFilter(unsigned int index);
	void removeAllFilters();

	void updateFilterType(std::uint32_t index);

	

private:
	std::shared_ptr<ShapeSettings> _settings;
	std::vector<std::shared_ptr<NoiseFilter> > _noiseFilters;
};

}	// ns proceduralPlanet