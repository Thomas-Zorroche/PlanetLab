#version 330 core


out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in vec3 vFragPos_os;
in vec3 vNormal_os;

uniform vec3 u_color;

void main()
{
    fFragColor = vec4(u_color, 1.0f);
}


