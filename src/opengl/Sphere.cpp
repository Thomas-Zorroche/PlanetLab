#include "opengl/Sphere.hpp"
#include "glm/glm.hpp"

#define _USE_MATH_DEFINES
#define NOMINAX // Avoid conflicts between min and max constants in Windef.h

#include <math.h>


Sphere::Sphere(float radius , int rings, int sector)
{
	build(radius, rings, sector);
}

void Sphere::build(float radius, int rings, int sector)
{
    GLfloat rcpLat = 1.f / (float)rings, rcpLong = 1.f / (float)sector;
    GLfloat dPhi = 2 * M_PI * rcpLat, dTheta = M_PI * rcpLong;

    std::vector<ShapeVertex> data;

    // Construit l'ensemble des vertex
    for (GLsizei j = 0; j <= sector; ++j) {
        GLfloat cosTheta = cos(-M_PI / 2 + j * dTheta);
        GLfloat sinTheta = sin(-M_PI / 2 + j * dTheta);

        for (GLsizei i = 0; i <= rings; ++i) {
            ShapeVertex vertex;

            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = 1.f - j * rcpLong;

            vertex.normal.x = sin(i * dPhi) * cosTheta;
            vertex.normal.y = sinTheta;
            vertex.normal.z = cos(i * dPhi) * cosTheta;

            vertex.position = radius * vertex.normal;

            data.push_back(vertex);
        }
    }

    _nVertexCount = rings * sector * 6;

    GLuint idx = 0;
    for (GLsizei j = 0; j < sector; ++j) {
        GLsizei offset = j * (rings + 1);
        for (GLsizei i = 0; i < rings; ++i) {
            _vertices.push_back(data[offset + i]);
            _vertices.push_back(data[offset + (i + 1)]);
            _vertices.push_back(data[offset + rings + 1 + (i + 1)]);
            _vertices.push_back(data[offset + i]);
            _vertices.push_back(data[offset + rings + 1 + (i + 1)]);
            _vertices.push_back(data[offset + i + rings + 1]);
        }
    }
}