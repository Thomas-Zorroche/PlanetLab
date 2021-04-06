#include "ShapeGenerator.hpp"

#include "ShapeSettings.hpp"
#include "noise/NoiseFilter.hpp"

ShapeGenerator::ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings)
	: _settings(shapeSettings), 
	_noiseFilters(std::vector<std::shared_ptr<NoiseFilter> >(_settings->noiseLayers().size(), nullptr))
{
	//for (size_t i = 0; i < _settings->noiseLayers().size(); i++)
	//{
	//	_noiseFilters[i] = std::make_shared<NoiseFilter>();
	//	_noiseFilters[i]->setSettings(_settings->noiseLayer(i)->noiseSettings());
	//}
}

glm::vec3 ShapeGenerator::calculatePointOnPlanet(const glm::vec3& pointOnUnitSphere) const
{
	float elevation = 0;
	for (size_t i = 0; i < _noiseFilters.size(); i++)
	{
		if (_settings->noiseLayer(i)->enabled())
		{
			elevation += _noiseFilters[i]->evaluate(pointOnUnitSphere);
		}
	}
	return pointOnUnitSphere * _settings->planetRadius() * (1 + elevation);
}

void ShapeGenerator::addFilter(const std::shared_ptr<NoiseFilter>& layer)
{
	_noiseFilters.push_back(layer);
}

void ShapeGenerator::removeLastFilter()
{
	_noiseFilters.pop_back();
}

void ShapeGenerator::removeFilter(unsigned int index)
{
	_noiseFilters.erase(_noiseFilters.begin() + index);
}
