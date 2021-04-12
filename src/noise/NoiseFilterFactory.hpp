#pragma once

#include "INoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include "NoiseFilter.hpp"
#include "RigidNoiseFilter.hpp"

#include <memory>

class NoiseFilterFactory
{
public:
	static INoiseFilter CreateNoiseFilter(const std::shared_ptr<NoiseSettings>& settings)
	{
		switch (settings->filterType())
		{
		case NoiseSettings::Simple:
			return SimpleNoiseFilter(settings);
			break;

		case NoiseSettings::Rigid:
			return RigidNoiseFilter(settings);
			break;
		}
	}
};