#pragma once

#include "NoiseFilter.hpp"
#include "NoiseSettings.hpp"

#include "NoiseFilter.hpp"
#include "RigidNoiseFilter.hpp"
#include "SimpleNoiseFilter.hpp"

#include <memory>

class NoiseFilterFactory
{
public:
	static std::shared_ptr<NoiseFilter> CreateNoiseFilter(const std::shared_ptr<NoiseSettings>& settings)
	{
		switch (settings->filterType())
		{
		case NoiseSettings::Simple:
			return std::make_shared<SimpleNoiseFilter>(settings);
			break;

		case NoiseSettings::Rigid:
			return std::make_shared<RigidNoiseFilter>(settings);
			break;
		}
	}
};