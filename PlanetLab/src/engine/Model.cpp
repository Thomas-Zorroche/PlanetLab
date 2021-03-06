#include "engine/Model.hpp"
#include "common.hpp"
#include "opengl/Texture.h"
#include "engine/ResourceManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

namespace PlanetLab
{

Model::Model(const std::string& path)
{
	LoadModel(path);
}

Model::Model(const std::shared_ptr<Mesh>& mesh)
    : _meshes { mesh }
{
}

Model::Model(const std::vector<std::shared_ptr<Mesh> >& meshes)
    : _meshes(meshes)
{
}


// draws the model, and thus all its meshes
void Model::Draw(std::shared_ptr<Shader>& shader)
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
        _meshes[i]->Draw(shader);
}

void Model::DrawPoints(int pointsSize) const
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
        _meshes[i]->drawPoints(pointsSize);
}


void Model::LoadModel(const std::string& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    _directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::string nameMesh = mesh->mName.C_Str();

        _meshes.push_back(std::make_shared<Mesh>(processMesh(mesh, scene, false)));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, bool IscBox)
{
    // data to fill
    std::vector<ShapeVertex> vertices;
    std::vector<unsigned int> indices;


    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ShapeVertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    const auto mat = scene->mMaterials[mesh->mMaterialIndex];
    aiString name;
    mat->Get(AI_MATKEY_NAME, name);

    // If material is already cached, return Mesh with this material
    const auto cachedMaterial = ResourceManager::Get().GetMaterial(name.C_Str());
    if (cachedMaterial != nullptr)
        return Mesh(vertices, cachedMaterial, indices);

    // Otherwise, create new material with textures
    // 1. diffuse maps
    aiString aiDiffusePath;
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &aiDiffusePath);
    std::string diffusePath("res/img/");
    diffusePath += aiDiffusePath.C_Str();
    // 2. specular maps
    aiString specularPath;
    mat->GetTexture(aiTextureType_SPECULAR, 0, &specularPath);
    // 3. normal maps
    aiString normalPath;
    mat->GetTexture(aiTextureType_HEIGHT, 0, &normalPath);

    // Cache the new Material
    const auto newMaterial = ResourceManager::Get().CacheBasicMaterial(name.C_Str(), diffusePath);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, newMaterial, indices);
}

int Model::getVerticesCount() const
{
    int verticesCount = 0;
    for (const auto& mesh : _meshes)
    {
        if (mesh->getVisibility())
            verticesCount += mesh->getVerticesCount();
    }
    return verticesCount;
}



/*
* Free the memory of all meshes
*/
void Model::Free()
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
        _meshes[i]->Free();
}


}
