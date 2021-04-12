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

	std::vector<std::shared_ptr<NoiseFilter> > noiseFilters();
	std::shared_ptr<NoiseFilter> noiseFilter(unsigned int index) const;
	std::shared_ptr<NoiseFilter>& noiseFilter(unsigned int index);

	void addFilter(const std::shared_ptr<NoiseFilter>& layer);
	void removeLastFilter();
	void removeFilter(unsigned int index);
	void removeAllFilters();

private:
	std::shared_ptr<ShapeSettings> _settings;
	std::vector<std::shared_ptr<NoiseFilter> > _noiseFilters;
};