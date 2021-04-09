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
	Init();
}

void Scene::Init()
{
	// Procedural Planet
	_planet = std::make_shared<Planet>();

	// Background Gradient (Simple Plane placed on camera far plane)
	_backgroundPlane = std::make_shared<BackgroundGradient>();

	// Load All Lights
	LightManager::Get().LoadAllLights();
}

void Scene::Draw(float viewportHeight, const std::shared_ptr<Application>& app)
{
	_backgroundPlane->Draw(viewportHeight, app->GetBackgroundColor());
	_planet->draw(app->IsWireframeMode());
}

void Scene::Free()
{
	ResourceManager::Get().DeleteAllResources();
}

