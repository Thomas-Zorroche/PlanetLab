#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"
#include "hud/ObserverHud.hpp"

#include "planets/ShapeSettings.hpp"
#include "planets/ColorSettings.hpp"
#include "planets/ShapeGenerator.hpp"

#include <memory>

class Planet
{
public:
	Planet(int resolution = 64);

	void draw();

private:
	void generatePlanet();
	void generateMesh();
	void checkHud();
	void generateColors();

private:
	int _resolution;

	std::shared_ptr<ShapeSettings> _shapeSettings;
	ColorSettings _colorSettings;
	ShapeGenerator _shapeGenerator;

	TerrainFace _terrainFaces[6];
	StaticMesh _staticMesh;

	ObserverHud _observer;
};