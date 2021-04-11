#pragma once

#include "GizmoAxis.hpp"

class Gizmo
{
public:
	Gizmo(float size = 50.0f, float thickness = 0.2f);

	void Draw();

private:
	float _size;
	float _thickness;
	GizmoAxis _axes[3];
};