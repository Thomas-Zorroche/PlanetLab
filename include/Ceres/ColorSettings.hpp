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

/**
* @brief Contains all color settings for a Ceres Planet (landmass and oceans)
*/
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
	// =====================================
	// Landmass settigns
	// =====================================

	/// Color of planet lands
	Color _landmassColor;
	/// Wheter to use a color ramp for landmass color
	bool _useLandmassRamp = false;
	/// Max steps in the color ramp
	const int _MAX_STEPS_COLOR = 5;
	/// ImGUI Color ramp object
	ImGradient _gradient = ImGradient(5);
	
	// =====================================
	// Ocean settings
	// =====================================

	/// Whether to use a different color for oceans
	bool _useOceanColor = false;
	/// Amount of depth in ocean
	float _oceanDepth = 0.0f;
	/// Ocean color
	Color _oceanColor = Color(0.219, 0.686, 0.784);

};

}	// ns Ceres