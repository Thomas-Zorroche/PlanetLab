#version 330 core


out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in vec3 vFragPos_os;
in vec3 vNormal_os;

uniform sampler2D texture_diffuse;

void main()
{
    // Texture
    //fFragColor = texture(texture_diffuse, vVertexTexcoords * uvScale) * vec4(finalColor, 1.0f);

    vec3 finalColor = vec3(0.0, 1.0, 0.0);
    fFragColor = vec4(finalColor, 1.0f);
}


