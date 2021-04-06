#include "engine/Scene.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Renderer.hpp"
#include "engine/Skybox.hpp"

#include "opengl/Shader.h"

#include "lighting/LightManager.hpp"
#include "lighting/PointLight.hpp"
#include "lighting/DirectionalLight.hpp"

#include "planets/Planet.hpp"

#include "io/IOManager.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>


Scene::Scene()
	:  _skybox(nullptr), _planet(nullptr)
{
	Init();
}

Scene::~Scene() {}

void Scene::Init()
{
	// Procedural Planet
	//=======================
	_planet = std::make_shared<Planet>();

	// Test : Load ini files
	// ======================
	std::string path = "res/scene/test.ini";
	if (!IOManager::get().open(path, _planet))
	{
		std::cout << "Error IO :: cannot open file " << path << std::endl;
	}

	// Load All Lights
	// =================
	LightManager::Get().LoadAllLights();

}

void Scene::Draw()
{
	_planet->draw();

	/*for (size_t i = 0; i < _staticMeshesCount; i++)
		_staticMeshes[i]->Draw();*/
}

void Scene::AddStaticMesh(const std::shared_ptr<StaticMesh>& mesh)
{
	_staticMeshes.push_back(mesh);
	_staticMeshesCount++;
}

void Scene::Free()
{
	// Free all static meshes
	// ==========================
	for (size_t i = 0; i < _staticMeshesCount; i++)
		_staticMeshes[i]->Free();

	// Free all textures 
	// ==========================
	ResourceManager::Get().DeleteAllResources();
}

