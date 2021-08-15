#include "engine/StaticMesh.hpp"
#include "engine/ResourceManager.hpp"
#include "lighting/LightManager.hpp"
#include "opengl/Shader.h"
#include "opengl/Mesh.hpp"
#include "engine/Renderer.hpp"
#include "editor/Application.hpp"
#include "editor/Editor.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace PlanetLab
{

// Default Constructor
StaticMesh::StaticMesh(const std::shared_ptr<Model>& model, const TransformLayout& transLayout, const std::string& shaderName)
	: _model(model), 
	_transformLayout(transLayout), 
	_shader(ResourceManager::Get().GetShader(shaderName)),
	_modelMatrix(glm::mat4(1.0f))
{
	// Translate the mesh to the correct location
	Translate(_transformLayout.Location());
	Rotate(_transformLayout.Rotation());
	Scale(_transformLayout.Scale());
}

// Constructor with one mesh
StaticMesh::StaticMesh(const std::shared_ptr<Mesh>& mesh, const TransformLayout& transLayout, const std::string& shaderName)
	: _model(std::make_shared<Model>(mesh)),
	_transformLayout(transLayout),
	_shader(ResourceManager::Get().GetShader(shaderName)),
	_modelMatrix(glm::mat4(1.0f))
{
	// Translate the mesh to the correct location
	Translate(_transformLayout.Location());
	Rotate(_transformLayout.Rotation());
	Scale(_transformLayout.Scale());
}

// Constructor with multiples meshes
StaticMesh::StaticMesh(const std::vector<std::shared_ptr<Mesh>>& meshes, const TransformLayout& transLayout, const std::string& shaderName)
	: _model(std::make_shared<Model>(meshes)), 
	_transformLayout(transLayout), 
	_shader(ResourceManager::Get().GetShader(shaderName)),
	_modelMatrix(glm::mat4(1.0f))
{
	// Translate the mesh to the correct location
	Translate(_transformLayout.Location());
	Rotate(_transformLayout.Rotation());
	Scale(_transformLayout.Scale());
}

void StaticMesh::Draw(bool hasWireframe, bool hideSurface)
{
	SendUniforms();

	if (!hideSurface)
		_model->Draw(_shader);

	if (hasWireframe && Editor::Get().getEditorSettings()->IsWireframeVisible())
	{
		auto shaderWireframe = ResourceManager::Get().GetShader("Wireframe");
		shaderWireframe->Bind();
		Renderer::Get().SendTransMatrixUniforms(_modelMatrix, shaderWireframe);
		shaderWireframe->Unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_model->Draw(shaderWireframe);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (hasWireframe && Editor::Get().getEditorSettings()->isPointsDisplayed())
	{
		auto shaderPoints = ResourceManager::Get().GetShader("Point");
		shaderPoints->Bind();
		Renderer::Get().SendTransMatrixUniforms(_modelMatrix, shaderPoints);
		_model->DrawPoints(Editor::Get().getEditorSettings()->getSizePoints());
		shaderPoints->Unbind();
	}
}

/*
* Transfromations
*/

void StaticMesh::Scale(float alpha)
{
	_modelMatrix = _modelMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(alpha));
}

void StaticMesh::SetScale(float alpha)
{
	_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(alpha));
}

void StaticMesh::Scale(const glm::vec3& vector)
{
	_modelMatrix = _modelMatrix * glm::scale(glm::mat4(1.0f), vector);
}

void StaticMesh::Translate(const glm::vec3& delta)
{
	_modelMatrix = _modelMatrix * glm::translate(glm::mat4(1.0f), delta);
}

void StaticMesh::Rotate(const glm::vec3& alpha)
{
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(alpha.x), glm::vec3(1, 0, 0));
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(alpha.y), glm::vec3(0, 1, 0));
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(alpha.z), glm::vec3(0, 0, 1));
}

/*
* Uniforms
*/
void StaticMesh::SendUniforms()
{
	_shader->Bind();

	// Send Transformations Matrixes
	Renderer::Get().SendTransMatrixUniforms(GetModelMatrix(), _shader);

	// Send Lights
	LightManager::Get().SendUniforms(_shader);

	_shader->Unbind();
}



/*
* Free Memory of model
*/
void StaticMesh::Free()
{
	_model->Free();
}

}
