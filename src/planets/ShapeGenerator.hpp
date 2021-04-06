#pragma once

#include "glm/glm.hpp"

#include <memory>
#include <vector>

class NoiseFilter;
class ShapeSettings;

class ShapeGenerator
{
public:
	ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings);

	glm::vec3 calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;

	void addFilter(const std::shared_ptr<NoiseFilter>& layer);
	void removeLastFilter();
	void removeFilter(unsigned int index);
	void removeAllFilters();

private:
	std::shared_ptr<ShapeSettings> _settings;
	std::vector<std::shared_ptr<NoiseFilter> > _noiseFilters;
};