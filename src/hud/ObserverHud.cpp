#include "ObserverHud.hpp"
#include "hud/hud.hpp"

ObserverHud::ObserverHud()
{

}

std::vector<ObsFlag> ObserverHud::checkHud(int& resolution, Color& color)
{
	std::vector<ObsFlag> flags;

	if (Hud::get().resolution() != resolution)
	{
		resolution = Hud::get().resolution();
		flags.push_back(ObsFlag::RESOLUTION);
	}

	if (Hud::get().colorPlanet() != color)
	{
		color = Hud::get().colorPlanet();
		flags.push_back(ObsFlag::COLOR);
	}

	return flags;
}