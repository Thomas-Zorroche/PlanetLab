#pragma once

#include "glm/glm.hpp"

class NoiseSettings
{
public:
	NoiseSettings() {};

	float strength() const { return _strength; }
	float roughness() const { return _roughness; }
	glm::vec3 center() const { return _center; }

	float& strength() { return _strength; }
	float& roughness() { return _roughness; }
	glm::vec3& center() { return _center; }

private:
	float _strength = 1.0f;
	float _roughness = 1.0f;
	glm::vec3 _center = glm::vec3(0, 0, 0);
};