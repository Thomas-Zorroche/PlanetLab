#version 330 core

out vec4 fFragColor;

uniform vec3 u_color;
uniform float u_viewportHeight;

void main()
{
    fFragColor = vec4(u_color * vec3(gl_FragCoord.y / u_viewportHeight), 1.0f);
}


