#pragma once

namespace PlanetLab
{

class ShapeSphere
{
public:
	ShapeSphere(float radius = 1.0f, int rings = 12, int sector = 24);

	const ShapeVertex* vertices() const {
		return &_vertices[0];

	}
	GLsizei vertexCount() const {
		return _nVertexCount;
	}

private:
	void build(float radius, int rings, int sector);

	std::vector<ShapeVertex> _vertices;
	GLsizei _nVertexCount;
};

}