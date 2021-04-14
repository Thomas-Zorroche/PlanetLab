#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexcoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform float u_maxElevation;

out vec2 vVertexTexcoords;
out vec3 vNormal_vs;
out vec3 vFragPos_vs;
out float elevation;

void main() 
{
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    // View Space
    vFragPos_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    
    vVertexTexcoords = aVertexTexcoords;

    elevation = clamp((length(aVertexPosition) - 1) / (u_maxElevation - 1), 0, 1);

    gl_Position = uMVPMatrix * vertexPosition;
}