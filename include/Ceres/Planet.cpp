#include "Planet.hpp"

#include "noise/NoiseFilter.hpp"
#include "noise/SimpleNoiseFilter.hpp"
#include "noise/NoiseSettings.hpp"
#include "noise/NoiseFilterFactory.hpp"

#include "engine/opengl/Mesh.hpp"

namespace Ceres
{

Planet::Planet(int resolution, bool visible)
	: _resolution(resolution),
	_visible(visible),
	_shapeSettings(std::make_shared<ShapeSettings>(1.0f /* radius */)),
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
		_terrainFaces[0].getMesh(), 
		_terrainFaces[1].getMesh(),
		_terrainFaces[2].getMesh(),
		_terrainFaces[3].getMesh(),
		_terrainFaces[4].getMesh(),
		_terrainFaces[5].getMesh(),
	}, PlanetLab::TransformLayout(glm::vec3(0)), "Planet")
{
	if (_visible)
		generatePlanet();
}

void Planet::draw()
{
	if (!_visible)
		return;

	if (_scaleOnLoading && _scaleLoadingAmount < 1)
	{
		_scaleLoadingAmount += _scaleLoadingSpeed;
		_staticMesh.SetScale(_scaleLoadingAmount);
	}

	// Planet spin
	rotate(glm::vec3(0, _rotationSpeed, 0));

	sendUniforms();
	_staticMesh.Draw(true, _hideSurface);
}

void Planet::sendUniforms()
{
	auto shader = _staticMesh.GetShader();
	shader->Bind();

	_colorSettings->sendUniforms(shader);
	shader->SetUniform1f("u_maxElevation", _maxElevation);

	shader->Unbind();
}

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
			if (face.getMaxElevation() > _maxElevation)
			{
				_maxElevation = face.getMaxElevation();
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
		face.getMesh()->setColor(_colorSettings->getLandmassColor());
	}
}

void Planet::generatePlanet()
{
	generateMesh();
	generateColors();
}

void Planet::update(ObserverFlag flag)
{
	switch (flag)
	{
		case ObserverFlag::RESOLUTION:
		{
			for (auto& face : _terrainFaces)
			{
				face.updateResolution(_resolution);
			}

			emitResolutionChanged(_resolution);
			break;
		}
		case ObserverFlag::COLOR:
		{
			generateColors();
			break;
		}
		case ObserverFlag::MESH:
		{
			generateMesh();
			emitMeshChanged();
			break;
		}
		case ObserverFlag::FACERENDERMASK:
		{
			emitResolutionChanged(_resolution);
			break;
		}
	}
}

std::shared_ptr<NoiseLayer> Planet::addNoiseLayer(unsigned int count)
{
	std::shared_ptr<NoiseLayer> layer = nullptr;
	for (size_t i = 0; i < count; i++)
	{
		layer = std::make_shared<NoiseLayer>(_shapeSettings->getNoiseLayers().size());
		_shapeSettings->addLayer(layer);
		_shapeGenerator->addFilter(NoiseFilterFactory::createNoiseFilter(layer->getNoiseSettings()));
	}
	return layer;
}

void Planet::removeLastNoiseLayer()
{
	_shapeSettings->removeLastLayer();
	_shapeGenerator->removeLastFilter();
}

void Planet::reset()
{
	// Remove all noise settings layers
	_shapeSettings->removeAllLayers();

	// Remove all noise filters (noise alogrithm)
	_shapeGenerator->removeAllFilters();

	// Remove colors
	_colorSettings->reset();

	_resolution = 64;
	_scaleLoadingAmount = 0.0f;
	_visible = true;

	update(ObserverFlag::RESOLUTION);
	update(ObserverFlag::COLOR);
	update(ObserverFlag::MESH);
}

void Planet::rotate(const glm::vec3& angles)
{
	_staticMesh.Rotate(angles);
}

void Planet::generateRandomPlanet()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Reseed
	for (std::size_t i = 0; i < _shapeGenerator->getNoiseFilters().size(); i++)
	{
		_shapeGenerator->getNoiseFilter(i)->reseed(seed);
	}

	// Colors
	_colorSettings->setRandomColors(seed);

	// Update Mesh
	update(ObserverFlag::MESH);
	update(ObserverFlag::COLOR);
}

int Planet::getVerticesCount() const
{
	return _staticMesh.getVerticesCount();
}

int Planet::getFacesCount() const
{
	int visibleFacesCount = 0;
	for (const TerrainFace& face : _terrainFaces)
	{
		if (face.getVisibility())
			visibleFacesCount++;
	}
	return (_resolution - 1) * (_resolution - 1) * visibleFacesCount;
}

// SIGNALS
void Planet::emitResolutionChanged(int resolution)
{
	_planetSubject.updateResolution(_resolution);
}

void Planet::emitMeshChanged()
{
	_planetSubject.updateMesh();
}

} // ns Ceres




