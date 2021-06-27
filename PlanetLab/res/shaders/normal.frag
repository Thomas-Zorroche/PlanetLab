#version 330 core

out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in vec3 vFragPos_os;
in vec3 vNormal_os;

uniform vec3 cameraPos;

uniform sampler2D texture_diffuse;
uniform float uvScale;

void main()
{
    // Normal and View direction in VIEW SPACE
    vec3 Normal_vs = normalize(vNormal_vs);
    vec3 viewDir_vs = normalize(-vFragPos_vs);

    // Color
    fFragColor = vec4(vNormal_vs, 1.0f);
}


