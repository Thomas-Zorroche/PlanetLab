#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "ui/BackgroundGradient.hpp"
#include "editor/Application.hpp"
#include "engine/Skybox.hpp"

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

	std::vector<std::string> facesSkybox
	{
		"right.png",
		"left.png",
		"top.png",
		"bottom.png",
		"front.png",
		"back.png"
	};
	_skybox = std::make_shared<Skybox>(facesSkybox);

	// Load All Lights
	LightManager::Get().LoadAllLights();
}


void Scene::Draw(float viewportHeight)
{
	_skybox->Draw();

	_planet->draw();

	if (Application::Get().GetEditor().IsAxisVisible())
		_gizmo.Draw();
}

void Scene::Free()
{
	ResourceManager::Get().DeleteAllResources();
}

} // ns Editor
