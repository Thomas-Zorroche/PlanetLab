#pragma once


#include "glm/glm.hpp"

#include <memory>

class NoiseFilter;
class ShapeSettings;

class ShapeGenerator
{
public:
	ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings);

	glm::vec3 calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;

private:
	std::shared_ptr<ShapeSettings> _settings;
	std::shared_ptr<NoiseFilter> _noiseFilter;
};