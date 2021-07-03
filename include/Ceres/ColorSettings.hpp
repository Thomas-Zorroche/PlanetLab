#pragma once

#include "engine/Color.hpp"
#include "imgui/imgui_color_gradient.h"

#include <memory>
#include <vector>

namespace PlanetLab
{
class Color;
class Shader;
}

namespace Ceres
{

class ColorSettings
{
public:
	ColorSettings(const PlanetLab::Color& color = PlanetLab::Color());
	
	const PlanetLab::Color& color() const;
	PlanetLab::Color& color();

	float GetOceanDepth() const { return _oceanDepth; }
	float& GetOceanDepth() { return _oceanDepth; }

	PlanetLab::Color GetOceanColor() const { return _oceanColor; }
	PlanetLab::Color& GetOceanColor() { return _oceanColor; }

	void SendUniforms(std::shared_ptr<PlanetLab::Shader>& shader);

	ImGradient& GetGradient() { return _gradient; }

	bool& GetUseLandmassRamp() { return _useLandmassRamp; }
	bool GetUseLandmassRamp() const { return _useLandmassRamp; }

	bool& GetUseOceanColor() { return _useOceanColor; }
	bool GetUseOceanColor() const { return _useOceanColor; }

	void SetRandomColors(std::uint32_t seed);

	void reset();

private:
	// Landmass settigns
	PlanetLab::Color _colorPlanet;
	bool _useLandmassRamp = false;
	const int _MAX_STEPS_COLOR = 5;
	ImGradient _gradient = ImGradient(5);

	// Ocean settings
	bool _useOceanColor = false;
	float _oceanDepth = 0.0f;
	PlanetLab::Color _oceanColor = PlanetLab::Color(0, 0, 1);

};

}	// ns Ceres