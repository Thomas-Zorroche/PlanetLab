#pragma once

#include <vector>
#include <memory>
#include "common.hpp"
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
	std::vector<ShapeVertex>& Vertices() { return _vertices; }
	const std::shared_ptr<Material>& MaterialPtr() const { return _material; }

	void UpdateGeometry(const std::vector<ShapeVertex>& vertices, const std::vector<unsigned int>& indices);

private:
	void SetupMesh(bool generateBuffers = true);
	void Clear();

private:
	std::vector<ShapeVertex>  _vertices;
	std::vector<unsigned int> _indices;
	
	unsigned int VAO, VBO, EBO;

	std::shared_ptr<Material> _material;

};