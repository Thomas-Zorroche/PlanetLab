#include "BackgroundGradient.hpp"
#include "opengl/Mesh.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Application.hpp"

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

void BackgroundGradient::Draw(float viewportHeight)
{
	_shader->Bind();
	glDisable(GL_DEPTH_TEST);

	// Color 
	const glm::vec3& color = (glm::vec3&)Application::Get().GetBackgroundColor();
	_shader->SetUniform3f("u_color", color);
	_shader->SetUniform1f("u_viewportHeight", viewportHeight);

	_mesh.Draw(_shader);

	glEnable(GL_DEPTH_TEST);
	_shader->Unbind();
}

