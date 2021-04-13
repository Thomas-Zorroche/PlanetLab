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

out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in float elevation;

uniform Material material;
uniform DirLight dirLight;

uniform vec3[2] u_colors;
uniform float[2] u_steps;

vec3 ComputeDirLight(Material material, DirLight dirLight, vec3 normal, vec3 viewDir);

void main()
{
    // Normal and View direction in VIEW SPACE
    vec3 Normal_vs = normalize(vNormal_vs);
    vec3 viewDir_vs = normalize(-vFragPos_vs);

    // Lighting
    vec3 finalColor = vec3(0.0f);
    finalColor += ComputeDirLight(material, dirLight, Normal_vs, viewDir_vs);

    // Color
    vec3 color = mix(u_colors[0], u_colors[1], smoothstep(u_steps[1], u_steps[0], elevation));
    fFragColor = vec4(color, 1.0);
    //fFragColor = vec4(finalColor, 1.0f);
}


vec3 ComputeDirLight(Material material, DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse * diffuseStrength;
    vec3 specular = light.specular * material.specular * specularStrength;

    return vec3(ambient + diffuse + specular) * light.intensity;
}

