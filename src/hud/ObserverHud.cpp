#include "ObserverHud.hpp"
#include "hud/hud.hpp"

#include "noise/NoiseSettings.hpp"


ObserverHud::ObserverHud()
{

}

std::vector<ObsFlag> ObserverHud::checkHud(int& resolution, Color& color, std::shared_ptr<ShapeSettings>& shapeSettings)
{
	std::vector<ObsFlag> flags;

	// RESOLUTION
	if (Hud::get().resolution() != resolution)
	{
		resolution = Hud::get().resolution();
		flags.push_back(ObsFlag::RESOLUTION);
	}

	// COLOR
	if (Hud::get().planetColor() != color)
	{
		color = Hud::get().planetColor();
		flags.push_back(ObsFlag::COLOR);
	}

	// RADIUS
	if (Hud::get().planetRadius() != shapeSettings->planetRadius())
	{
		shapeSettings->planetRadius() = Hud::get().planetRadius();
		flags.push_back(ObsFlag::RADIUS);
	}

	// NOISE STRENGTH 
	if (Hud::get().noiseStrength() != shapeSettings->noiseSettings()->strength())
	{
		shapeSettings->noiseSettings()->strength() = Hud::get().noiseStrength();
		flags.push_back(ObsFlag::NOISE);
	}

	// NOISE ROUGHNESS 
	if (Hud::get().noiseRoughness() != shapeSettings->noiseSettings()->roughness())
	{
		shapeSettings->noiseSettings()->roughness() = Hud::get().noiseRoughness();
		flags.push_back(ObsFlag::NOISE);
	}

	return flags;
}