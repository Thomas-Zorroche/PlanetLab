#include "ObserverHud.hpp"
#include "hud/hud.hpp"

ObserverHud::ObserverHud()
{

}

std::vector<ObsFlag> ObserverHud::checkHud(int& resolution)
{
	std::vector<ObsFlag> flags;

	if (Hud::get().resolution() != resolution)
	{
		resolution = Hud::get().resolution();
		flags.push_back(ObsFlag::RESOLUTION);
	}

	return flags;
}