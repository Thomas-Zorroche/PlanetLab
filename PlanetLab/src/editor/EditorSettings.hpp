#pragma once

namespace PlanetLab
{

class EditorSettings
{
public:
	EditorSettings() = default;
	~EditorSettings() = default;

	// Wireframe
	bool IsWireframeVisible() const { return _showWireframe; }
	bool& IsWireframeVisible() { return _showWireframe; }
	void SetWireframeVisible(bool showWireframe) { _showWireframe = showWireframe; }

	// Axis
	bool IsAxisVisible() const { return _showAxis; }
	bool& IsAxisVisible() { return _showAxis; }
	void SetAxisVisible(bool showAxis) { _showAxis = showAxis; }

	// Stats
	bool IsStatsVisible() const { return _showStatistics; }
	bool& IsStatsVisible() { return _showStatistics; }
	void SetStatsVisible(bool showStats) { _showStatistics = showStats; }

	// Skybox
	bool isSkyboxDisplayed() const { return _skyboxDisplayed; }
	bool& isSkyboxDisplayed() { return _skyboxDisplayed; }

	// Skybox
	bool isPointsDisplayed() const { return _pointsDisplayed; }
	bool& isPointsDisplayed() { return _pointsDisplayed; }

	int getSizePoints() const { return _sizePoints; }
	int& getSizePoints() { return _sizePoints; }

	void setDarkThemeMode() const;


private:
	bool _showWireframe = false;
	
	bool _showAxis = false;

	bool _showStatistics = true;

	bool _skyboxDisplayed = true;

	bool _pointsDisplayed = false;

	int _sizePoints = 1;
};

}   // ns editor