#version 330 core

layout(location = 0) in vec3 aVertexPosition;

uniform mat4 uMVPMatrix;

void main() {
    
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vertexPosition.xyz *= 1.001; 

    gl_Position = uMVPMatrix * vertexPosition;

}