#pragma once

#include "opengl/Mesh.hpp"
#include "glm/glm.hpp"
#include <memory>

class TerrainFace
{
public:
	TerrainFace(int resolution, const glm::vec3& localUp);

	void constructMesh();
	
	std::shared_ptr<Mesh>& mesh() { return _mesh; }

private:
	std::shared_ptr<Mesh> _mesh;
	int _resolution;
	glm::vec3 _localUp;

	glm::vec3 _axisA;
	glm::vec3 _axisB;
};