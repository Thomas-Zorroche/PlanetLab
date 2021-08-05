#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"

#include "ShapeSettings.hpp"
#include "ColorSettings.hpp"
#include "ShapeGenerator.hpp"

#include "observer/PlanetSubject.hpp"

#include "imgui/imgui_color_gradient.h"


namespace Ceres
{

enum class FaceRenderMask
{
	All = 0,
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};

enum class ObserverFlag
{
	RESOLUTION = 0,
	COLOR,
	MESH,
	FACERENDERMASK
};


class Planet
{
public:
	Planet(int resolution = 64);

	void draw();

	std::shared_ptr<ShapeSettings> shapeSettings() { return _shapeSettings; }
	std::shared_ptr<ColorSettings> colorSettings() { return _colorSettings; }
	std::shared_ptr<ShapeGenerator> shapeGenerator() { return _shapeGenerator; }

	int& resolution() { return _resolution; }

	void update(ObserverFlag flag);

	void reset();

	void Rotate(const glm::vec3& angles);

	FaceRenderMask& getFaceRenderMask() { return _faceRenderMask; }

	void RandomGenerate();

	int getVerticesCount() const;
	int getFacesCount() const;

	PlanetSubject& getPlanetSubject() { return _planetSubject; }

	void addNoiseLayer(unsigned int count = 1);

	void removeLastNoiseLayer();



private:
	void generatePlanet();
	void generateMesh();
	void generateColors();
	void sendUniforms();


/*
* Signals
* Do not use it. Use EMIT keyword.
*/
private:
	void emitResolutionChanged(int resolution);


private:
	int _resolution;

	std::shared_ptr<ShapeSettings> _shapeSettings;
	std::shared_ptr<ColorSettings> _colorSettings;
	std::shared_ptr<ShapeGenerator> _shapeGenerator;

	TerrainFace _terrainFaces[6];
	PlanetLab::StaticMesh _staticMesh;

	FaceRenderMask _faceRenderMask = FaceRenderMask::All;

	float _maxElevation = 0.0f;

	PlanetSubject _planetSubject;
};

#define EMIT_ResolutionChanged(resolution) emitResolutionChanged(resolution)

} // ns Procedural Planet