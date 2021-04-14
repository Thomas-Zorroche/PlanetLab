#version 330 core

struct Material
{
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight 
{
    float intensity;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define MAX_COLOR_STEPS 5

out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in float elevation;
in float depth;

uniform Material material;
uniform DirLight dirLight;

uniform vec3[MAX_COLOR_STEPS] u_colors;
uniform float[MAX_COLOR_STEPS] u_steps;
uniform int u_colorStepCount;
uniform float u_oceanDepth;
uniform vec3 u_oceanColor;

vec3 ComputeDirLight(vec3 diffuseColorRamp, Material material, DirLight dirLight, vec3 normal, vec3 viewDir);

void main()
{
    // Normal and View direction in VIEW SPACE
    vec3 Normal_vs = normalize(vNormal_vs);
    vec3 viewDir_vs = normalize(-vFragPos_vs);

    vec3 diffuseColorRamp;
    // Ocean
    if (depth < 0)
    {
        float inverseDepth = clamp(1 - abs(u_oceanDepth * depth), 0, 1);
        diffuseColorRamp = inverseDepth * u_oceanColor;
    }
    // Land
    else
    {
        diffuseColorRamp = mix(u_colors[0], u_colors[1], smoothstep(u_steps[0], u_steps[1], elevation));
        for (int i = 2; i <= u_colorStepCount; i++)
            diffuseColorRamp = mix(diffuseColorRamp, u_colors[i], smoothstep(u_steps[i - 1], u_steps[i], elevation));
    }

    // Lighting
    vec3 finalColor = vec3(0.0f);
    finalColor += ComputeDirLight(diffuseColorRamp, material, dirLight, Normal_vs, viewDir_vs);

    // Color
    fFragColor = vec4(finalColor, 1.0f);
}


vec3 ComputeDirLight(vec3 diffuseColorRamp, Material material, DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diffuseColorRamp * diffuseStrength;
    vec3 specular = light.specular * material.specular * specularStrength;

    return vec3(ambient + diffuse + specular) * light.intensity;
}

