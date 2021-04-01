#pragma once

#include "ShapeSettings.hpp"
#include "glm/glm.hpp"

#include <memory>

class ShapeGenerator
{
public:
	ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings);

	glm::vec3 calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const;

private:
	std::shared_ptr<ShapeSettings> _settings;
};