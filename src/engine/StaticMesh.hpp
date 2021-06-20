#pragma once

#include "engine/Model.hpp"
#include "opengl/Shader.h"

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class CollisionManager;

class TransformLayout
{
public:
	TransformLayout(const glm::vec3& loc = glm::vec3(0, 0, 0), 
		const glm::vec3& rot = glm::vec3(0, 0, 0), 
		float scale = 1.0f, 
		float uvScale = 1.0f)
		: _location(loc), _rotation(rot), _scale(scale), _uvScale(uvScale) {}

	glm::vec3 Location() const { return _location; }
	glm::vec3 Rotation() const { return _rotation; }
	float Scale() const { return _scale; }
	float UvScale() const { return _uvScale; }

private:
	glm::vec3 _location;
	glm::vec3 _rotation;
	float _scale;
	float _uvScale;
};


class StaticMesh
{
public:
	// Default Constructor
	StaticMesh(const std::shared_ptr<Model>& model = std::make_shared<Model>(), const TransformLayout& transLayout = TransformLayout(), 
		const std::string& shaderName = "DefaultLighting");

	// Constructor with one mesh
	StaticMesh(const std::shared_ptr<Mesh>& mesh, const TransformLayout& transLayout = TransformLayout(),
		const std::string& shaderName = "DefaultLighting");
	
	// Constructor with multiples meshes
	StaticMesh(const std::vector<std::shared_ptr<Mesh>>& meshes, const TransformLayout& transLayout = TransformLayout(), 
		const std::string& shaderName = "DefaultLighting");

	void Draw(bool hasWireframe = true);

	glm::mat4 GetModelMatrix() const { return _modelMatrix; }
	std::shared_ptr<Shader>& GetShader() { return _shader; }

	void Translate(const glm::vec3& delta);
	void Scale(float alpha);
	void Scale(const glm::vec3& vector);
	void Rotate(const glm::vec3& alpha);

	unsigned int GetVAO() const { return _model->GetVAO(); }

	void Free();

private:
	void SendUniforms();

	std::shared_ptr<Model> _model;

	TransformLayout _transformLayout;
	std::shared_ptr<Shader> _shader;
	glm::mat4 _modelMatrix;
};

