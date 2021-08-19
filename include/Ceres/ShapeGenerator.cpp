#include "ShapeGenerator.hpp"

#include "noise/NoiseLayer.hpp"
#include "noise/NoiseLayerFactory.hpp"

namespace Ceres
{

ShapeGenerator::ShapeGenerator()
	: _noiseLayers(std::vector<std::shared_ptr<NoiseLayer> >())
{

}

float ShapeGenerator::calculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const
{
	float firstLayerValue = 0;
	float elevation = 0;

	if (_noiseLayers.size() > 0)
	{
		firstLayerValue = _noiseLayers[0]->evaluate(pointOnUnitSphere); // TODO: put this in the if ?
		if (_noiseLayers[0]->isEnabled())
		{
			elevation = firstLayerValue;
		}
	}

	for (size_t i = 1; i < _noiseLayers.size(); i++)
	{
		if (_noiseLayers[i]->isEnabled())
		{
			float mask = (_noiseLayers[i]->firstLayerUsedAsMask()) ? firstLayerValue : 1;
			elevation += _noiseLayers[i]->evaluate(pointOnUnitSphere) * mask;
		}
	}
	return elevation;
}

float ShapeGenerator::getScaledElevation(float unscaledElevation) const
{
	float elevation = std::max(0.0f, unscaledElevation);
	elevation = _planetRadius * (1 + elevation);
	return elevation;
}

std::vector<std::shared_ptr<NoiseLayer> > ShapeGenerator::getNoiseLayers()
{
	return _noiseLayers;
}

std::shared_ptr<NoiseLayer> ShapeGenerator::getNoiseLayer(unsigned int index) const
{
	if (index >= _noiseLayers.size())
		return nullptr;
	return _noiseLayers[index];
}

void ShapeGenerator::addLayer(const std::shared_ptr<NoiseLayer>& layer)
{
	_noiseLayers.push_back(layer);
}

void ShapeGenerator::removeLastLayer()
{
	_noiseLayers.pop_back();
}

void ShapeGenerator::removeLayer(unsigned int index)
{
	_noiseLayers.erase(_noiseLayers.begin() + index);
}

void ShapeGenerator::removeAllLayers()
{
	int size = _noiseLayers.size();
	for (size_t i = 0; i < size; i++)
	{
		removeLastLayer();
	}
}

std::shared_ptr<NoiseLayer> ShapeGenerator::updateLayerType(std::uint32_t index)
{
	if (index >= _noiseLayers.size()) 
	{
		CERES_ERROR("Update Layer Tpye: index out of bounds");
		return nullptr;
	}

	_noiseLayers[index] = NoiseLayerFactory::createNoiseLayer(index, _noiseLayers[index]->getNoiseSettings());
	return _noiseLayers[index];
}

}	// ns Ceres
