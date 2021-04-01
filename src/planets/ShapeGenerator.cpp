#include "ShapeGenerator.hpp"

ShapeGenerator::ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings)
	: _settings(shapeSettings)
{

}

glm::vec3 ShapeGenerator::calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
	return pointOnUnitSphere * _settings->planetRadius();
}
