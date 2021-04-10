#include "lighting/BaseLight.hpp"

BaseLight::BaseLight(LightType type, float intensity, const glm::vec3& color, bool electricity)
	: _type(type), _intensity(intensity), _color(color),
	  _ambient(0.3f, 0.3f, 0.3f), _diffuse(color), _specular(0.5, 0.5, 0.5)
{

}

// Getters & Setters
float BaseLight::Intensity() const
{
	return _intensity;
}
float& BaseLight::Intensity()
{
	return _intensity;
}
void BaseLight::SetIntensity(float intensity)
{
	_intensity = intensity;
}
const glm::vec3& BaseLight::Ambient() const
{
	return _ambient; 
}
void BaseLight::SetAmbient(const glm::vec3& ambient)
{
	_ambient = ambient;
}
void BaseLight::SetAmbient(float value)
{
	_ambient = glm::vec3(value);
}

const glm::vec3& BaseLight::Diffuse() const
{
	return _diffuse;
}
void BaseLight::SetDiffuse(const glm::vec3& diffuse)
{
	_diffuse = diffuse;
}

const glm::vec3& BaseLight::Specular() const
{
	return _specular;
}
void BaseLight::SetSpecular(const glm::vec3& specular)
{
	_specular = specular;
}
