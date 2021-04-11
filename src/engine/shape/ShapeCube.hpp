#pragma once

#include <vector>
#include "common.hpp"
#include "glm/glm.hpp"

class ShapeCube
{
public:
	ShapeCube(const glm::vec3& origin = glm::vec3(0, 0, 0), float w = 1.0f, float h = 1.0f, float d = 1.0f);

	const std::vector<ShapeVertex>& Vertices() const { return _vertices; }
	const std::vector<unsigned int>& Indices() const { return _indices; }

	void GenerateVertices();

private:
	glm::vec3 _origin;
	float _w;
	float _h;
	float _d;

	std::vector<ShapeVertex> _vertices;
	std::vector<unsigned int> _indices;
};
