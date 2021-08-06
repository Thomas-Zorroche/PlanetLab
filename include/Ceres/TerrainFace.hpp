#pragma once

#include "engine/opengl/Mesh.hpp"
#include "ShapeGenerator.hpp"

namespace Ceres
{

/**
* @brief Terrain Face, 6 of them form the planet sphere
*/
class TerrainFace
{
public:
	TerrainFace(const std::shared_ptr<ShapeGenerator>& shapeGenerator, int resolution, const glm::vec3& localUp);

	void setVisibility(bool visibility) { _mesh->setVisibility(visibility); }
	bool getVisibility() const { return _mesh->getVisibility(); }

	const std::shared_ptr<PlanetLab::Mesh>& getMesh() const { return _mesh; }
	std::shared_ptr<PlanetLab::Mesh> getMesh() { return _mesh; }
	
	float getMaxElevation() const { return _maxElevation; }
	
	/// Create grid mesh
	void constructMesh();
	
	/// Update resolution and re construct the mesh
	void updateResolution(int resolution);

private:
	std::shared_ptr<PlanetLab::Mesh> _mesh;
	
	int _resolution;
	
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;

	std::shared_ptr<ShapeGenerator> _shapeGenerator;

	float _maxElevation = 1.0f;
};

}	// ns Ceres