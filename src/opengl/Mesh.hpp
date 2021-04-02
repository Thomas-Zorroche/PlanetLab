#pragma once

#include <vector>
#include <memory>
#include "common.hpp"
#include "engine/Color.hpp"
#include "opengl/Shader.h"
#include "engine/Material.hpp"

class Material;

class Mesh
{
public:
	Mesh(const std::vector<ShapeVertex> & vertices = std::vector<ShapeVertex>(),
		 const std::shared_ptr<Material>& material = std::make_shared<Material>(),
		 const std::vector<unsigned int>& indices  = std::vector<unsigned int>());

	void Free();

	void Draw(std::shared_ptr<Shader>& shader, bool IsParticuleInstance = false, int countParticules = 0) const;

	unsigned int GetVAO() const { return VAO; }
	const std::vector<ShapeVertex>& Vertices() const { return _vertices; }
	const std::vector<unsigned int>& Indices() const { return _indices; }
	std::vector<ShapeVertex>& Vertices() { return _vertices; }
	ShapeVertex& Vertex(unsigned int index);
	const ShapeVertex& Vertex(unsigned int index) const;
	const std::shared_ptr<Material>& MaterialPtr() const { return _material; }

	void UpdateGeometry(const std::vector<ShapeVertex>& vertices, const std::vector<unsigned int>& indices);
	void setColor(const Color& color);

private:
	void UpdateVBO();
	void SetupMesh(bool generateBuffers = true);
	void Clear();
	void recalculateNormals();
	glm::vec3 calculateSurfaceNormal(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);


private:
	std::vector<ShapeVertex>  _vertices;
	std::vector<unsigned int> _indices;
	
	unsigned int VAO, VBO, EBO;

	std::shared_ptr<Material> _material;

};