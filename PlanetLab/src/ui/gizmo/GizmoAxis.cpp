#include "GizmoAxis.hpp"

#include "engine/Model.hpp"
#include "engine/shape/ShapeCube.hpp"
#include "engine/opengl/Mesh.hpp"

#include <memory>

namespace PlanetLab
{

GizmoAxis::GizmoAxis(Axis axis, float size, float thickness)
	: _axis(axis),
	_size(size),
	_thickness(thickness),
	_staticMesh()
{
	InitColor();
	InitMesh();
}

void GizmoAxis::InitColor()
{
	switch (_axis)
	{
	case Axis::X:
		_color = Color(1.0, 0.0, 0.0);
		break;
	case Axis::Y:
		_color = Color(0.0, 1.0, 0.0);
		break;
	case Axis::Z:
		_color = Color(0.0, 0.0, 1.0);
		break;
	}
}

void GizmoAxis::InitMesh()
{
	ShapeCube cube;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
		cube.Vertices(), 
		std::make_shared<Material>(),
		cube.Indices()
	);

	mesh->MaterialPtr()->SetDiffuse(_color.vector());

	_staticMesh = StaticMesh(std::make_shared<Model>(mesh), TransformLayout(), "Gizmo");

	ScaleOnAxis();
}

void GizmoAxis::ScaleOnAxis()
{
	switch (_axis)
	{
	case Axis::X:
		_staticMesh.Scale(glm::vec3(_size, _thickness, _thickness));
		break;
	case Axis::Y:
		_staticMesh.Scale(glm::vec3(_thickness, _size, _thickness));
		break;
	case Axis::Z:
		_staticMesh.Rotate(glm::vec3(90, 0, 0));
		_staticMesh.Scale(glm::vec3(_thickness, _size, _thickness));
		break;
	}
}

void GizmoAxis::Draw()
{
	auto shader = _staticMesh.GetShader();
	shader->Bind();
	shader->SetUniform3f("u_color", _color.vector());
	shader->Unbind();
	
	_staticMesh.Draw(false); // false --> no wireframe
}

}