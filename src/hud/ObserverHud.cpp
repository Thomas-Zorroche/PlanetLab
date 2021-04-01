#include "ObserverHud.hpp"
#include "hud/hud.hpp"

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

	return flags;
}