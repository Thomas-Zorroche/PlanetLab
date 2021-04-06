#include "Planet.hpp"
#include "glm/glm.hpp"
#include "opengl/Mesh.hpp"
#include <vector>


Planet::Planet(int resolution)
	: _resolution(resolution),
	_shapeSettings(std::make_shared<ShapeSettings>(1.0f)),
	_colorSettings(std::make_shared<ColorSettings>()),
	_shapeGenerator(_shapeSettings),
	_terrainFaces{ 
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  1,  0)), // UP
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0, -1,  0)), // DOWN
		TerrainFace(_shapeGenerator, resolution, glm::vec3(-1,  0,  0)), // LEFT
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 1,  0,  0)), // RIGHT
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  0, -1)), // FORWARD
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  0,  1))  // BACK
	},
	_staticMesh({ 
		_terrainFaces[0].mesh(), 
		_terrainFaces[1].mesh(),
		_terrainFaces[2].mesh(),
		_terrainFaces[3].mesh(),
		_terrainFaces[4].mesh(),
		_terrainFaces[5].mesh(),
	})
{
	generatePlanet();
}

void Planet::draw()
{
	//checkHud();
	_staticMesh.Draw();
}

/* Generate Fonctions */
void Planet::generateMesh()
{
	for (TerrainFace& face : _terrainFaces)
	{
		face.constructMesh();
	}
}

void Planet::generateColors()
{
	for (TerrainFace& face : _terrainFaces)
	{
		face.mesh()->setColor(_colorSettings->color());
	}
}

void Planet::generatePlanet()
{
	generateMesh();
	generateColors();
}


void Planet::update(ObserverFlag flag)
{
	switch (flag)
	{
		case ObserverFlag::RESOLUTION:
		{
			for (auto& face : _terrainFaces)
				face.updateResolution(_resolution);
		}
		case ObserverFlag::COLOR:
		{
			generateColors();
		}
		case ObserverFlag::RADIUS:
		{
			generateMesh();
		}
		case ObserverFlag::NOISE:
		{
			generateMesh();
		}
	}
}



