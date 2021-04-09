#include "opengl/Mesh.hpp"
#include "common.hpp"
#include "opengl/Shader.h"
#include "opengl/Texture.h"
#include <string>
#include <iostream>
#include "engine/Material.hpp"
#include "engine/ResourceManager.hpp"

Mesh::Mesh(const std::vector<ShapeVertex>& vertices, const std::shared_ptr<Material>& material,
           const std::vector<unsigned int>& indices)
    : _vertices(vertices), _indices(indices), _material(material)
{
    if (!vertices.empty())
        SetupMesh();
}

void Mesh::Free()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::SetupMesh(bool generateBuffers)
{
    // Generate Buffers
    if (generateBuffers)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(ShapeVertex), &_vertices[0], GL_DYNAMIC_DRAW);

    if (!_indices.empty())
    {
        if (generateBuffers)
        {
            glGenBuffers(1, &EBO);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_DYNAMIC_DRAW);
    }

    // Vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader>& shader) const
{
    shader->Bind();

    _material->SendMaterialUniform(shader);

    // Textures 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (_material->TextureCount() > 0)
    {
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, _material->GetParameterTexture(0));
    }

    // draw mesh
    glBindVertexArray(VAO);
    if (_indices.empty())
        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    else
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader->Unbind();
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_BLEND);
}

void Mesh::UpdateGeometry(const std::vector<ShapeVertex>& vertices, const std::vector<unsigned int>& indices)
{
    bool empty = _vertices.empty();

    // Clear all data and reaplace it by the parameters
    Clear();
    _vertices = vertices;
    _indices = indices;

    recalculateNormals();
    
    // Generate Buffers Data
    if (empty)
    {
        SetupMesh(empty);
    }
    // Update Buffers Data
    else
    {
        SetupMesh(false);
    }

}

void Mesh::UpdateVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    {
        void* ptrVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptrVBO, &_vertices, sizeof(_vertices));
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    {
        void* ptrEBO = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptrEBO, &_indices, sizeof(_indices));
    }
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void Mesh::Clear()
{
    _vertices.clear();
    _indices.clear();
}

void Mesh::setColor(const Color& color)
{
    _material->setDiffuse(color.vector());
}

ShapeVertex& Mesh::Vertex(unsigned int index)
{
    if (index > _vertices.size())
        throw std::string("Mesh Vertices : Out of bounds");
    
    return _vertices[index];
}

const ShapeVertex& Mesh::Vertex(unsigned int index) const
{
    if (index > _vertices.size())
        throw std::string("Mesh Vertices : Out of bounds");

    return _vertices[index];
}

void Mesh::recalculateNormals()
{
    for (size_t i = 0; i < _indices.size(); i += 3)
    {
        glm::vec3 faceNormal = calculateSurfaceNormal(
            _vertices[_indices[i]].position,
            _vertices[_indices[i + 1]].position,
            _vertices[_indices[i + 2]].position
        );

        _vertices[_indices[i]].normal = faceNormal;
        _vertices[_indices[i + 1]].normal = faceNormal;
        _vertices[_indices[i + 2]].normal = faceNormal;
    }
}


glm::vec3 Mesh::calculateSurfaceNormal(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C)
{
    glm::vec3 u(B - A);
    glm::vec3 v(C - A);

    return glm::normalize(glm::cross(u, v));
}

