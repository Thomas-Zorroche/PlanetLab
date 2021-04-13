#pragma once

#include "opengl/Mesh.hpp"
#include "glm/glm.hpp"
#include "planets/ShapeGenerator.hpp"
#include <memory>

class TerrainFace
{
public:
	TerrainFace(const std::shared_ptr<ShapeGenerator>& shapeGenerator, int resolution, const glm::vec3& localUp);

	void constructMesh();
	void updateResolution(int resolution);
	
	std::shared_ptr<Mesh>& mesh() { return _mesh; }

	void setVisibility(bool visibility) 
	{ 
		_mesh->setVisibility(visibility);
	}

private:
	std::shared_ptr<Mesh> _mesh;
	int _resolution;
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;

	std::shared_ptr<ShapeGenerator> _shapeGenerator;
};