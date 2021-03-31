#include "Planet.hpp"
#include "glm/glm.hpp"
#include "opengl/Mesh.hpp"
#include <vector>

Planet::Planet(int resolution)
	: _resolution(resolution),
	_terrainFaces{ 
		TerrainFace(resolution, glm::vec3( 0,  1,  0)), // UP
		TerrainFace(resolution, glm::vec3( 0, -1,  0)), // DOWN
		TerrainFace(resolution, glm::vec3(-1,  0,  0)), // LEFT
		TerrainFace(resolution, glm::vec3( 1,  0,  0)), // RIGHT
		TerrainFace(resolution, glm::vec3( 0,  0, -1)), // FORWARD
		TerrainFace(resolution, glm::vec3( 0,  0,  1))  // BACK
	},
	_staticMesh({ 
		std::make_shared<Mesh>(_terrainFaces[0].mesh()), 
		std::make_shared<Mesh>(_terrainFaces[1].mesh()),
		std::make_shared<Mesh>(_terrainFaces[2].mesh()),
		std::make_shared<Mesh>(_terrainFaces[3].mesh()),
		std::make_shared<Mesh>(_terrainFaces[4].mesh()),
		std::make_shared<Mesh>(_terrainFaces[5].mesh()),
	})
{
	generateMesh();
}

void Planet::generateMesh()
{
	for (TerrainFace& face : _terrainFaces)
	{
		face.constructMesh();
	}
}

void Planet::draw()
{
	_staticMesh.Draw();
}
