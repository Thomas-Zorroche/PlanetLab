#pragma once

#include "glm/glm.hpp"
#include <memory>
#include <vector>

enum class LightType
{
	POINT = 0, DIR, SPOT, AREA
};

class BaseLight
{
public:
	BaseLight(LightType type,
			  float intensity = 1.0f, 
			  const glm::vec3& color = glm::vec3(1.0, 1.0, 1.0),
			  bool electricity = true);

	virtual ~BaseLight() {}

	float Intensity() const;
	float& Intensity();
	void SetIntensity(float intensity);

	const glm::vec3& Ambient() const;
	void SetAmbient(const glm::vec3& ambient);
	void SetAmbient(float ambient);

	const glm::vec3& Diffuse() const;
	void SetDiffuse(const glm::vec3& diffuse);

	const glm::vec3& Specular() const;
	void SetSpecular(const glm::vec3& specular);

	LightType Type() const { return _type; }

	virtual std::vector<glm::vec3> GetSpecialData() = 0;


private:
	LightType _type;
	float _intensity;
	glm::vec3 _color;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;

	glm::mat4 _modelMatrix = glm::mat4(1.0f);

};

using BaseLightPtr = std::shared_ptr<BaseLight>;