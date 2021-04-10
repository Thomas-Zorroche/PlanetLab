#include "lighting/LightManager.hpp"

#include <string>
#include <iostream>

#include "lighting/DirectionalLight.hpp"
#include "engine/Renderer.hpp"
#include "lighting/PointLight.hpp"

const int LightManager::POINT_LIGHTS_COUNT = 7;

void LightManager::SendUniforms(const std::shared_ptr<Shader>& shader)
{
	shader->SetUniform1f("dirLight.intensity", _light->Intensity());
	shader->SetUniform3f("dirLight.ambient", _light->Ambient());
	shader->SetUniform3f("dirLight.diffuse", _light->Diffuse());
	shader->SetUniform3f("dirLight.specular", _light->Specular());

	glm::mat4 modelLight = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 0), glm::vec3(0, 1, 0));
	glm::vec4 LightDirection = Renderer::Get().View() * modelLight * glm::vec4(1, 1, 1, 0);

	shader->SetUniform3f("dirLight.direction", LightDirection.x, LightDirection.y, LightDirection.z);
}


void LightManager::LoadAllLights()
{
	// Directional Light
	// =========================================================================
	_light = std::make_shared<DirectionalLight>(
		0.8f,						// Intensity
		glm::vec3(0.8, 0.8, 0.8),   // Color
		glm::vec3(1, 1, 1));		// Direction
}
