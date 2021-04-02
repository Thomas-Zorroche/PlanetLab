#include "ShapeGenerator.hpp"

ShapeGenerator::ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings)
	: _settings(shapeSettings), _noiseFilter(NoiseFilter())
{

}

glm::vec3 ShapeGenerator::calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
	float elevation = _noiseFilter.evaluate(pointOnUnitSphere);
	return pointOnUnitSphere * _settings->planetRadius() * (1 + elevation);
}
