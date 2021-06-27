#include "Gizmo.hpp"

namespace PlanetLab
{

Gizmo::Gizmo(float size, float thickness)
	: _size(size),
	_thickness(thickness),
	_axes {
		GizmoAxis(Axis::X, _size, _thickness),
		GizmoAxis(Axis::Y, _size, _thickness),
		GizmoAxis(Axis::Z, _size, _thickness)
	}
{
	
}

void Gizmo::Draw()
{
	for (size_t i = 0; i < 3; i++)
	{
		_axes[i].Draw();
	}
}

}