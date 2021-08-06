#pragma once

#include "NoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include "NoiseFilter.hpp"
#include "RigidNoiseFilter.hpp"
#include "SimpleNoiseFilter.hpp"

namespace Ceres
{

/**
* @brief Factory for creating Noise Filter based on it type
*/
class NoiseFilterFactory
{
public:

	/// Create NoiseFilter linked with a noise settings
	static std::shared_ptr<NoiseFilter> createNoiseFilter(const std::shared_ptr<NoiseSettings>& settings)
	{
		switch (settings->filterType)
		{
		case FilterType::Simple:
			return std::make_shared<SimpleNoiseFilter>(settings);
			break;

		case FilterType::Rigid:
			return std::make_shared<RigidNoiseFilter>(settings);
			break;
		}
	}
};

}	// ns proceduralPlanet