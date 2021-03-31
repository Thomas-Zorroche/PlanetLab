#version 330 core

struct Material
{
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    float intensity;
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
}; 

struct DirLight 
{
    float intensity;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define POINT_LIGHTS_COUNT 1

out vec4 fFragColor;

in vec3 vNormal_vs;
in vec3 vFragPos_vs;
in vec2 vVertexTexcoords;
in vec3 vFragPos_os;
in vec3 vNormal_os;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];

uniform vec3 cameraPos;

uniform sampler2D texture_diffuse;
uniform float uvScale;

vec3 ComputeDirLight(Material material, DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 ComputePointLight(Material material, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    // Normal and View direction in VIEW SPACE
    vec3 Normal_vs = normalize(vNormal_vs);
    vec3 viewDir_vs = normalize(-vFragPos_vs);

    // Lighting
    vec3 finalColor = vec3(0.0f);
    finalColor += ComputeDirLight(material, dirLight, Normal_vs, viewDir_vs);

    for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
        finalColor += ComputePointLight(material, pointLights[i], Normal_vs, vFragPos_vs, viewDir_vs);

    // Texture
    //fFragColor = texture(texture_diffuse, vVertexTexcoords * uvScale) * vec4(finalColor, 1.0f);
    fFragColor = vec4(finalColor, 1.0f);

}


vec3 ComputeDirLight(Material material, DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);

    vec3 reflectDir = reflect(-lightDir, normal);

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.2), material.shininess);

    vec3 ambient = light.ambient * 0.1 * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse * diffuseStrength;
    vec3 specular = light.specular * material.specular * specularStrength;

    return vec3(ambient + diffuse + specular);
}

vec3 ComputePointLight(Material material, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float distance = length(light.position - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 reflectDir = reflect(-lightDir, normal);

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float attenuation = 1.0 / (1.0f + light.linear * distance + light.quadratic * (distance * distance));  
    
    vec3 ambient = light.ambient * material.ambient * attenuation * light.intensity;
    vec3 diffuse = light.diffuse * material.diffuse * diffuseStrength * attenuation * light.intensity;
    vec3 specular = light.specular * material.specular * specularStrength * attenuation * light.intensity;

    return vec3(ambient + diffuse + specular);
}
