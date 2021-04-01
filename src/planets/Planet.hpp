#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"
#include "hud/ObserverHud.hpp"

#include "planets/ShapeSettings.hpp"
#include "planets/ColorSettings.hpp"

class Planet
{
public:
	Planet(int resolution = 64);

	void draw();

private:
	void generateMesh();
	void checkHud();
	void generateColors();

private:
	int _resolution;

	TerrainFace _terrainFaces[6];
	StaticMesh _staticMesh;

	ObserverHud _observer;

	ShapeSettings _shapeSettings;
	ColorSettings _colorSettings;
};