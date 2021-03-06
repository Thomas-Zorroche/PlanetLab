#pragma once

#include "TerrainFace.hpp"
#include "engine/StaticMesh.hpp"

#include "ColorSettings.hpp"
#include "ShapeGenerator.hpp"

#include "observer/PlanetSubject.hpp"

#include "imgui/imgui_color_gradient.h"


namespace Ceres
{

/**
* @brief Which face will be rendered
*/
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


/**
* @brief Ceres Planet Class
*/
class Planet
{
public:
	/// Defautl resolution of 64
	Planet(int resolution = 64, bool visible = true);

	// Accessors

	const std::shared_ptr<ColorSettings>& getColorSettings() const { return _colorSettings; }
	std::shared_ptr<ColorSettings> getColorSettings() { return _colorSettings; }

	const std::shared_ptr<ShapeGenerator>& getShapeGenerator() const { return _shapeGenerator; }
	std::shared_ptr<ShapeGenerator> getShapeGenerator() { return _shapeGenerator; }

	int getResolution() const { return _resolution; }
	int& getResolution() { return _resolution; }

	float getRotationSpeed() const { return _rotationSpeed; }
	float& getRotationSpeed() { return _rotationSpeed; }

	FaceRenderMask& getFaceRenderMask() { return _faceRenderMask; }
	PlanetSubject& getPlanetSubject() { return _planetSubject; }
	
	int getVerticesCount() const;
	int getFacesCount() const;

	void setScaleOnLoading(bool scale) { _scaleOnLoading = scale; }

	void setVisible(bool visible) { _visible = visible; }
	bool isVisible() const { return _visible; }

	bool isSurfaceHidden() const { return _hideSurface; }
	bool& isSurfaceHidden() { return _hideSurface; }

	// Public methods

	/// Draw static mesh planet
	void draw();
	/// Update planet depending given flag
	void update(ObserverFlag flag);
	/// Reset all planet settings with default values
	void reset();
	/// Rotate planet by given angle
	void rotate(const glm::vec3& angles);
	/// Change randomly ONLY seed and colors
	void generateRandomPlanet();
	/// Add noise layer. Return last noise layer added
	std::shared_ptr<NoiseLayer> addNoiseLayer(unsigned int count = 1);
	/// Remove last noise layer
	void removeLastNoiseLayer();

private:
	/// Generate planet mesh and colors
	void generatePlanet();
	/// Generate planet static mesh
	void generateMesh();
	/// Generate planet colors
	void generateColors();
	/// Send colors settings uniforms and max elevation value
	void sendUniforms();

private:
	void emitResolutionChanged(int resolution);
	void emitMeshChanged();


private:
	/// Resolution fo planet, amount of subdivisions per face
	int _resolution = 64;

	/// Whether to draw the planet (surface + wireframe)
	bool _visible = true;

	/// Whether to draw the surface
	bool _hideSurface = false;

	/// Contains ocean and landmass colors 
	std::shared_ptr<ColorSettings> _colorSettings;

	/// Responsible to apply noise on the shape
	std::shared_ptr<ShapeGenerator> _shapeGenerator;

	/// All 6 faces that form the sphere
	TerrainFace _terrainFaces[6];

	/// Planet static mesh
	PlanetLab::StaticMesh _staticMesh;

	/// Which face will be rendered
	FaceRenderMask _faceRenderMask = FaceRenderMask::All;

	/// Max elevation form the ground
	float _maxElevation = 0.0f;

	/// Subject of the Observer pattern
	PlanetSubject _planetSubject;

	/// Planet's spin 
	float _rotationSpeed = 0.02f;

	/// Whether to scale on loading
	bool _scaleOnLoading = false;
	
	/// Amount of animation scale on loading planet
	float _scaleLoadingAmount = 0.1f;

	/// Speed of scale loading animation 
	float _scaleLoadingSpeed = 0.02f;
};

} // ns Ceres