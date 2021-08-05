#pragma once

#include "engine/Color.hpp"
#include "imgui/imgui_color_gradient.h"
#include "Color/Color.hpp"

namespace PlanetLab
{
class Shader;
}

namespace Ceres
{

class ColorSettings
{
public:
	ColorSettings(const Color& color = Color());

	const Color& getLandmassColor() const { return _landmassColor; }
	Color& getLandmassColor() { return _landmassColor; }

	Color getOceanColor() const { return _oceanColor; }
	Color& getOceanColor() { return _oceanColor; }

	float getOceanDepth() const { return _oceanDepth; }
	float& getOceanDepth() { return _oceanDepth; }

	bool& getUseLandmassRamp() { return _useLandmassRamp; }
	bool getUseLandmassRamp() const { return _useLandmassRamp; }

	bool& getUseOceanColor() { return _useOceanColor; }
	bool getUseOceanColor() const { return _useOceanColor; }

	ImGradient& getGradient() { return _gradient; }

	/// Sets random colors for landmass and ocean
	void setRandomColors(std::uint32_t seed, unsigned int steps = 4);

	/// Replace ColorRamp marks by parameters marks
	void setColorMarks(const std::list<ImGradientMark>& marks);

	/// Send all settings to parameter shader
	void sendUniforms(std::shared_ptr<PlanetLab::Shader>& shader);

	/// Reset all settings with default value
	void reset();

private:
	// Landmass settigns
	Color _landmassColor;
	bool _useLandmassRamp = false;
	const int _MAX_STEPS_COLOR = 5;
	ImGradient _gradient = ImGradient(5);

	// Ocean settings
	bool _useOceanColor = false;
	float _oceanDepth = 0.0f;
	Color _oceanColor = Color(0, 0, 0.8);

};

}	// ns Ceres