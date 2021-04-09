#include "BackgroundGradient.hpp"
#include "opengl/Mesh.hpp"
#include "engine/ResourceManager.hpp"

#include <vector>

BackgroundGradient::BackgroundGradient()
	: _mesh(Mesh()), _shader(ResourceManager::Get().GetShader("BackgroundGradient"))
{
	// Create simple quad and place it at the far clip plane (2D)
	std::vector<ShapeVertex> vertices {
		ShapeVertex(-1, -1),
		ShapeVertex(-1,  1),
		ShapeVertex( 1,  1),
		ShapeVertex( 1, -1),
	};
	std::vector<unsigned int> indices{
		0, 1, 2,
		0, 2, 3
	};

	_mesh.UpdateGeometry(vertices, indices);
}

void BackgroundGradient::Draw(float viewportHeight, const Color& backgroundColor)
{
	_shader->Bind();
	glDisable(GL_DEPTH_TEST);

	// Color Background
	_shader->SetUniform3f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b);
	_shader->SetUniform1f("u_viewportHeight", viewportHeight);

	_mesh.Draw(_shader);

	glEnable(GL_DEPTH_TEST);
	_shader->Unbind();
}

