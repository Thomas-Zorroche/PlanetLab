#include "ShapeGenerator.hpp"

#include "ShapeSettings.hpp"
#include "noise/NoiseFilter.hpp"
#include "noise/NoiseFilterFactory.hpp"

namespace Ceres
{

ShapeGenerator::ShapeGenerator(const std::shared_ptr<ShapeSettings>& shapeSettings)
	: _settings(shapeSettings), 
	_noiseFilters(std::vector<std::shared_ptr<NoiseFilter> >(_settings->getNoiseLayers().size(), nullptr))
{

}

float ShapeGenerator::calculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const
{
	float firstLayerValue = 0;
	float elevation = 0;

	if (_noiseFilters.size() > 0)
	{
		firstLayerValue = _noiseFilters[0]->evaluate(pointOnUnitSphere);
		if (_settings->getNoiseLayer(0)->isEnabled())
		{
			elevation = firstLayerValue;
		}
	}

	for (size_t i = 1; i < _noiseFilters.size(); i++)
	{
		if (_settings->getNoiseLayer(i)->isEnabled())
		{
			float mask = (_settings->getNoiseLayer(i)->isUsingFirstLayerAsMask()) ? firstLayerValue : 1;
			elevation += _noiseFilters[i]->evaluate(pointOnUnitSphere) * mask;
		}
	}
	return elevation;
}

float ShapeGenerator::getScaledElevation(float unscaledElevation) const
{
	float elevation = std::max(0.0f, unscaledElevation);
	elevation = _settings->planetRadius() * (1 + elevation);
	return elevation;
}

std::vector<std::shared_ptr<NoiseFilter> > ShapeGenerator::getNoiseFilters()
{
	return _noiseFilters;
}

std::shared_ptr<NoiseFilter> ShapeGenerator::getNoiseFilter(unsigned int index) const
{
	if (index >= _noiseFilters.size())
		return nullptr;
	return _noiseFilters[index];
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

void ShapeGenerator::removeAllFilters()
{
	int size = _noiseFilters.size();
	for (size_t i = 0; i < size; i++)
	{
		removeLastFilter();
	}
}

void ShapeGenerator::updateFilterType(std::uint32_t index)
{
	if (index >= _noiseFilters.size()) 
	{
		CERES_ERROR("Update Filter Tpye: index out of bounds");
		return;
	}

	_noiseFilters[index] = NoiseFilterFactory::createNoiseFilter(_settings->getNoiseLayer(index)->getNoiseSettings());
}

}	// ns Ceres
