#pragma once

#include <vector>
#include "engine/Color.hpp"

enum ObsFlag
{
	RESOLUTION = 0,
	COLOR
};


class ObserverHud
{
public:
	ObserverHud();

	std::vector<ObsFlag> checkHud(int& resolution, Color& color);

private:

};
