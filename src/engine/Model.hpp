# pragma once 

#include <vector>
#include <string>
#include <memory>

#include "opengl/Mesh.hpp"
#include "opengl/Texture.h"
#include "opengl/Shader.h"
#include "common.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	// Import a 3D Model with Assimp
	Model(const std::string& path);

	// Build a Static Mesh with a vector of meshes
	Model(const std::vector<std::shared_ptr<Mesh> >& meshes);

	// Default
	Model();

	void Draw(std::shared_ptr<Shader>& shader, bool isParticuleInstance = false, int countParticule = 0);

	// Retrieve mesh vao (when there is a single mesh in the model)
	// Useful for particule system
	unsigned int GetVAO() const { return _meshes[0]->GetVAO(); }

	void Free();

private:
	std::vector<std::shared_ptr<Mesh> > _meshes;
	std::string _directory;


	void LoadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, bool IscBox);
};