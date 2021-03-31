#include "engine/StaticMesh.hpp"
#include "engine/ResourceManager.hpp"
#include "lighting/LightManager.hpp"
#include "opengl/Shader.h"
#include "opengl/Mesh.hpp"
#include "engine/Renderer.hpp"
#include "hud/Hud.hpp"

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


StaticMesh::StaticMesh(const std::shared_ptr<Model>& model, const TransformLayout& transLayout, const std::string& shaderName)
	: _model(model), 
	_transformLayout(transLayout), 
	_shader(ResourceManager::Get().GetShader(shaderName)),
	_modelMatrix(glm::mat4(1.0f))
{
	// Translate the mesh to the correct location
	Translate(_transformLayout.Location(), false);
	Rotate(_transformLayout.Rotation(), false);
	Scale(_transformLayout.Scale(), false);
}

StaticMesh::StaticMesh(const std::vector<std::shared_ptr<Mesh>>& meshes, const TransformLayout& transLayout, const std::string& shaderName)
	: _model(std::make_shared<Model>(meshes)), 
	_transformLayout(transLayout), 
	_shader(ResourceManager::Get().GetShader(shaderName)),
	_modelMatrix(glm::mat4(1.0f))
{

}

void StaticMesh::Draw(bool isParticuleInstance, int countParticule)
{
	SendUniforms();
	_model->Draw(_shader, isParticuleInstance, countParticule);
	if (Hud::get().wireframeMode())
	{
		auto shaderWireframe = ResourceManager::Get().GetShader("Wireframe");
		shaderWireframe->Bind();
		Renderer::Get().SendTransMatrixUniforms(_modelMatrix, shaderWireframe);
		shaderWireframe->Unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonOffset(0, -1),
		_model->Draw(shaderWireframe, isParticuleInstance, countParticule);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/*
* Transfromations
*/

void StaticMesh::Scale(float alpha, bool dynamic)
{
	_modelMatrix = _modelMatrix * glm::scale(glm::mat4(1.0f), glm::vec3(alpha));
}

void StaticMesh::Translate(const glm::vec3& delta, bool dynamic)
{
	_modelMatrix = _modelMatrix * glm::translate(glm::mat4(1.0f), delta);
}

void StaticMesh::Rotate(const glm::vec3& alpha, bool dynamic)
{
	_modelMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(alpha.x), glm::vec3(1, 0, 0));
	_modelMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(alpha.y), glm::vec3(0, 1, 0));
	_modelMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(alpha.z), glm::vec3(0, 0, 1));
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

