#pragma once

#include "proceduralPlanet/Color.hpp"
#include "engine/StaticMesh.hpp"

enum class Axis
{
	X = 0,
	Y,
	Z
};

class GizmoAxis
{
public:
	GizmoAxis(Axis axis, float size = 50.0f, float thickness = 0.2f);

	void Draw();

private:
	void InitColor();

	void InitMesh();
	
	void ScaleOnAxis();

private:
	Axis _axis;
	StaticMesh _staticMesh;
	Color _color;

	float _size;
	float _thickness;
};