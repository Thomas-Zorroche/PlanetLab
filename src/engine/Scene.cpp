#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Skybox.hpp"
#include "engine/BackgroundGradient.hpp"
#include "engine/Application.hpp"

#include "lighting/LightManager.hpp"
#include "planets/Planet.hpp"

#include "io/IOManager.hpp"

#include <memory>

namespace editor 
{

using namespace proceduralPlanet;

Scene::Scene()
	: _gizmo(2, 0.005)
{
	// Procedural Planet
	_planet = std::make_shared<Planet>();
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
