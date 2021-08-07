#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Skybox.hpp"
#include "ui/BackgroundGradient.hpp"
#include "editor/Application.hpp"

#include "lighting/LightManager.hpp"
#include "ceres/Planet.hpp"

#include "io/IOManager.hpp"

namespace PlanetLab
{

Scene::Scene()
{
	// Procedural Planet
	_planet = std::make_shared<Ceres::Planet>();
	Application::Get().AppendPlanet(_planet);
	_planet->setScaleOnLoading(true);
	_planet->setVisible(false);


	// Background Gradient (Simple Plane placed on camera far plane)
	_backgroundPlane = std::make_shared<BackgroundGradient>();

	// Load All Lights
	LightManager::Get().LoadAllLights();
}


void Scene::Draw(float viewportHeight)
{
	_backgroundPlane->Draw(viewportHeight);

	_planet->draw();

	if (Application::Get().GetEditor().IsAxisVisible())
		_gizmo.Draw();
}

void Scene::Free()
{
	ResourceManager::Get().DeleteAllResources();
}

} // ns Editor
