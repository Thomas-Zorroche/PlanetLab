#pragma once

#include <vector>
#include <memory>

#include "BaseLight.hpp"
#include "../opengl/Shader.h"

namespace PlanetLab
{

class PointLight;
class DirectionalLight;

class LightManager
{
public:
	static LightManager& Get()
	{
		static LightManager instance;
		return instance;
	}
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

	void LoadAllLights();

	void SendUniforms(const std::shared_ptr<Shader>& shader);
	
	BaseLightPtr GetLight() { return _light; }

private:
	LightManager() = default;
	~LightManager() = default;

	const static int POINT_LIGHTS_COUNT;

	BaseLightPtr _light;

	unsigned int _pointLightsCount = 0;
	unsigned int _dirLightsCount = 0;

};

}

