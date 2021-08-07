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

	// Axis
	bool IsStatsVisible() const { return _showStatistics; }
	bool& IsStatsVisible() { return _showStatistics; }
	void SetStatsVisible(bool showStats) { _showStatistics = showStats; }

private:
	bool _showWireframe = false;
	
	bool _showAxis = false;

	bool _showStatistics = true;
};

}   // ns editor