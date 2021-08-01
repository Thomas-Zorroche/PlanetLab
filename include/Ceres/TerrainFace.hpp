#pragma once

#include "engine/opengl/Mesh.hpp"
#include "ShapeGenerator.hpp"

namespace Ceres
{

class TerrainFace
{
public:
	TerrainFace(const std::shared_ptr<ShapeGenerator>& shapeGenerator, int resolution, const glm::vec3& localUp);

	void constructMesh();
	void updateResolution(int resolution);
	
	std::shared_ptr<PlanetLab::Mesh>& mesh() { return _mesh; }

	void setVisibility(bool visibility) { _mesh->setVisibility(visibility); }
	bool getVisibility() const { return _mesh->getVisibility(); }

	float maxElevation() const { return _maxElevation; }

private:
	std::shared_ptr<PlanetLab::Mesh> _mesh;
	int _resolution;
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;

	std::shared_ptr<ShapeGenerator> _shapeGenerator;

	float _maxElevation = 1.0f;
};

}	// ns proceduralPlanet