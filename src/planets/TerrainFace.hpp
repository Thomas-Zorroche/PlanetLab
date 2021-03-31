#pragma once

#include "opengl/Mesh.hpp"
#include "glm/glm.hpp"

class TerrainFace
{
public:
	TerrainFace(int resolution, const glm::vec3& localUp);

	void constructMesh();
	
	const Mesh& mesh() { return _mesh; }

private:
	Mesh _mesh;
	int _resolution;
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;
};