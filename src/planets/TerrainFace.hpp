#pragma once

#include "opengl/Mesh.hpp"
#include "glm/glm.hpp"
#include "planets/ShapeGenerator.hpp"
#include <memory>

class TerrainFace
{
public:
	TerrainFace(const ShapeGenerator& shapeGenerator, int resolution, const glm::vec3& localUp);

	void constructMesh();
	void updateResolution(int resolution);
	
	std::shared_ptr<Mesh>& mesh() { return _mesh; }


private:
	std::shared_ptr<Mesh> _mesh;
	int _resolution;
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;

	ShapeGenerator _shapeGenerator;
};