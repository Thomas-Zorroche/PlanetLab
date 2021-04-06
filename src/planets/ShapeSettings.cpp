#include "ShapeSettings.hpp"

#include "noise/NoiseSettings.hpp"


ShapeSettings::ShapeSettings(float radius)
	: _planetRadius(radius),
	_noiseLayers(std::vector<std::shared_ptr<NoiseLayer> > ()) {}

float& ShapeSettings::planetRadius() 
{ 
	return _planetRadius; 
}
float ShapeSettings::planetRadius() const 
{ 
	return _planetRadius; 
}

std::vector<std::shared_ptr<NoiseLayer> > ShapeSettings::noiseLayers()
{ 
	return _noiseLayers; 
}

std::shared_ptr<NoiseLayer> ShapeSettings::noiseLayer(unsigned int index)
{
	if (index > _noiseLayers.size())
		return nullptr;
	return _noiseLayers[index];
}

void ShapeSettings::updateLayers(int layersCount)
{
	int layersDiffCount = layersCount - _noiseLayers.size();
	if (layersDiffCount > 0)
	{
		for (size_t i = 0; i < layersDiffCount; i++)
		{
			addLayer(std::make_shared<NoiseLayer>());
		}
	}
	else
	{
		for (size_t i = 0; i < abs(layersDiffCount); i++)
		{
			removeLastLayer();
		}
	}
}

void ShapeSettings::addLayer(const std::shared_ptr<NoiseLayer>& layer)
{
	_noiseLayers.push_back(layer);
}

void ShapeSettings::removeLastLayer()
{
	_noiseLayers.pop_back();
}

void ShapeSettings::removeLayer(unsigned int index)
{
	_noiseLayers.erase(_noiseLayers.begin() + index);
}


/*
* Noise Layer
*/
NoiseLayer::NoiseLayer(const std::shared_ptr<NoiseSettings>& settings)
	: _noiseSettings(settings), _enabled(true)
{

}