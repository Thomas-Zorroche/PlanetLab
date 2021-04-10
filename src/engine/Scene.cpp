#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Skybox.hpp"
#include "engine/BackgroundGradient.hpp"
#include "engine/Application.hpp"

#include "lighting/LightManager.hpp"
#include "planets/Planet.hpp"

#include <memory>


Scene::Scene()
{
	// Procedural Planet
	_planet = std::make_shared<Planet>();
	Application::Get().AppendPlanet(_planet);

	// Background Gradient (Simple Plane placed on camera far plane)
	_backgroundPlane = std::make_shared<BackgroundGradient>();

	// Load All Lights
	LightManager::Get().LoadAllLights();
}


void Scene::Draw(float viewportHeight)
{
	_backgroundPlane->Draw(viewportHeight);
	_planet->draw();
}

void Scene::Free()
{
	ResourceManager::Get().DeleteAllResources();
}

