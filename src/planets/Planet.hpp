#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"
#include "hud/ObserverHud.hpp"

class Planet
{
public:
	Planet(int resolution = 64);

	void draw();

private:
	void generateMesh();
	void checkHud();

private:
	int _resolution;

	TerrainFace _terrainFaces[6];
	StaticMesh _staticMesh;

	ObserverHud _observer;
};