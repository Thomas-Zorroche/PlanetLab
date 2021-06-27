#include "Planet.hpp"
#include "glm/glm.hpp"
#include "engine/opengl/Mesh.hpp"

#include "noise/NoiseFilter.hpp"
#include "noise/SimpleNoiseFilter.hpp"
#include "noise/NoiseSettings.hpp"

#include "io/IOManager.hpp"

#include <vector>
#include <random>
#include <chrono>

namespace Ceres
{

Planet::Planet(int resolution)
	: _resolution(resolution),
	_shapeSettings(std::make_shared<ShapeSettings>(1.0f)),
	_colorSettings(std::make_shared<ColorSettings>()),
	_shapeGenerator(std::make_shared<ShapeGenerator>(_shapeSettings)),
	_terrainFaces{ 
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  1,  0)), // UP
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0, -1,  0)), // DOWN
		TerrainFace(_shapeGenerator, resolution, glm::vec3(-1,  0,  0)), // LEFT
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 1,  0,  0)), // RIGHT
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  0, -1)), // FORWARD
		TerrainFace(_shapeGenerator, resolution, glm::vec3( 0,  0,  1))  // BACK
	},
	_staticMesh({ 
		_terrainFaces[0].mesh(), 
		_terrainFaces[1].mesh(),
		_terrainFaces[2].mesh(),
		_terrainFaces[3].mesh(),
		_terrainFaces[4].mesh(),
		_terrainFaces[5].mesh(),
	}, PlanetLab::TransformLayout(glm::vec3(0)), "Planet")
{
	generatePlanet();
}

void Planet::draw()
{
	sendUniforms();
	_staticMesh.Draw();
}

void Planet::sendUniforms()
{
	auto shader = _staticMesh.GetShader();
	shader->Bind();

	_colorSettings->SendUniforms(shader);
	shader->SetUniform1f("u_maxElevation", _maxElevation);

	shader->Unbind();
}

/* Generate Fonctions */
void Planet::generateMesh()
{
	std::size_t i = 0;
	_maxElevation = 0;
	for (TerrainFace& face : _terrainFaces)
	{
		if (_faceRenderMask == FaceRenderMask::All || (int)_faceRenderMask - 1 == i)
		{
			face.setVisibility(true);
			face.constructMesh();
			if (face.maxElevation() > _maxElevation)
			{
				_maxElevation = face.maxElevation();
			}
		}
		else
		{
			face.setVisibility(false);
		}
		i++;
	}
}

void Planet::generateColors()
{
	for (TerrainFace& face : _terrainFaces)
	{
		face.mesh()->setColor(_colorSettings->color());
	}
}

void Planet::generatePlanet()
{
	generateMesh();
	generateColors();
}

void Planet::update(ObserverFlag flag)
{
	PlanetLab::IOManager::get().setUnsavedValues();

	switch (flag)
	{
		case ObserverFlag::RESOLUTION:
		{
			for (auto& face : _terrainFaces)
				face.updateResolution(_resolution);
		}
		case ObserverFlag::COLOR:
		{
			generateColors();
		}
		case ObserverFlag::MESH:
		{
			generateMesh();
		}
	}
}

void Planet::updateNoiseLayersCount(int noiseLayersCount)
{
	int layersDiffCount = noiseLayersCount - _shapeSettings->noiseLayers().size();

	// Add layers and filters
	if (layersDiffCount > 0)
	{
		for (size_t i = 0; i < layersDiffCount; i++)
		{
			auto layer = std::make_shared<NoiseLayer>();
			std::shared_ptr<NoiseFilter> filter = std::make_shared<SimpleNoiseFilter>(layer->noiseSettings());

			_shapeSettings->addLayer(layer);
			_shapeGenerator->addFilter(filter);
		}
	}
	// Remove layers and filters
	else
	{
		for (size_t i = 0; i < abs(layersDiffCount); i++)
		{
			_shapeSettings->removeLastLayer();
			_shapeGenerator->removeLastFilter();
		}
	}
}

void Planet::reset()
{
	_shapeSettings->removeAllLayers();
	_shapeGenerator->removeAllFilters();
	update(ObserverFlag::MESH);
}

void Planet::Rotate(const glm::vec3& angles)
{
	_staticMesh.Rotate(angles);
}

void Planet::RandomGenerate()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Reseed
	for (std::size_t i = 0; i < _shapeGenerator->noiseFilters().size(); i++)
	{
		_shapeGenerator->noiseFilter(i)->Reseed(seed);
	}

	// Colors
	_colorSettings->SetRandomColors(seed);

	// Update Mesh
	PlanetLab::Application::Get().Update(ObserverFlag::MESH);
	PlanetLab::Application::Get().Update(ObserverFlag::COLOR);
}

} // ns Procedural Planet




