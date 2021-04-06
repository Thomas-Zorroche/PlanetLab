#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"

#include "planets/ShapeSettings.hpp"
#include "planets/ColorSettings.hpp"
#include "planets/ShapeGenerator.hpp"

#include <memory>

enum ObserverFlag
{
	RESOLUTION = 0,
	COLOR,
	RADIUS,
	NOISE
};

class Planet
{
public:
	Planet(int resolution = 64);

	void draw();

	std::shared_ptr<ShapeSettings> shapeSettings() { return _shapeSettings; }
	std::shared_ptr<ColorSettings> colorSettings() { return _colorSettings; }

	int& resolution() { return _resolution; }

	void update(ObserverFlag flag);

private:
	void generatePlanet();
	void generateMesh();
	void generateColors();

private:
	int _resolution;

	std::shared_ptr<ShapeSettings> _shapeSettings;
	std::shared_ptr<ColorSettings> _colorSettings;
	ShapeGenerator _shapeGenerator;

	TerrainFace _terrainFaces[6];
	StaticMesh _staticMesh;
};