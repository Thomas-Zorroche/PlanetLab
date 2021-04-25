#pragma once

#include "NoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include "NoiseFilter.hpp"
#include "RigidNoiseFilter.hpp"
#include "SimpleNoiseFilter.hpp"

#include <memory>

namespace proceduralPlanet
{

class NoiseFilterFactory
{
public:
	static std::shared_ptr<NoiseFilter> CreateNoiseFilter(const std::shared_ptr<NoiseSettings>& settings)
	{
		switch (settings->GetFilterType())
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