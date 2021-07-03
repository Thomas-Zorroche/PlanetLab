#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Skybox.hpp"
#include "ui/BackgroundGradient.hpp"
#include "editor/Application.hpp"

#include "lighting/LightManager.hpp"
#include "ceres/Planet.hpp"

#include "io/IOManager.hpp"

#include <memory>

namespace PlanetLab
{

Scene::Scene()
	: _gizmo(2, 0.005)
{
	// Procedural Planet
	_planet = std::make_shared<Ceres::Planet>();
	IOManager::get().open("res/scene/Emma.ini", _planet);
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
	if (Application::Get().GetEditor().IsAxisVisible())
		_gizmo.Draw();
}

void Scene::Free()
{
	ResourceManager::Get().DeleteAllResources();
}

} // ns Editor