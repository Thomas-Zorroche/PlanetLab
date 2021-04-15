#pragma once

#include "engine/Color.hpp"
#include "editor/imgui_color_gradient.h"

#include <memory>
#include <vector>

class Color;
class Shader;

class ColorSettings
{
public:
	ColorSettings(const Color& color = Color());
	
	const Color& color() const;
	Color& color();

	float GetOceanDepth() const { return _oceanDepth; }
	float& GetOceanDepth() { return _oceanDepth; }

	Color GetOceanColor() const { return _oceanColor; }
	Color& GetOceanColor() { return _oceanColor; }

	void SendUniforms(std::shared_ptr<Shader>& shader);

	ImGradient& GetGradient() { return _gradient; }

	bool& GetUseLandmassRamp() { return _useLandmassRamp; }
	bool GetUseLandmassRamp() const { return _useLandmassRamp; }

	bool& GetUseOceanColor() { return _useOceanColor; }
	bool GetUseOceanColor() const { return _useOceanColor; }

private:
	// Landmass settigns
	Color _colorPlanet;
	bool _useLandmassRamp = false;
	const int _MAX_STEPS_COLOR = 5;
	ImGradient _gradient = ImGradient(5);

	// Ocean settings
	bool _useOceanColor = false;
	float _oceanDepth = 0.0f;
	Color _oceanColor = Color(0, 0, 1);

};