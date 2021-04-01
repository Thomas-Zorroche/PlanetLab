#pragma once

#include <vector>
#include <memory>
#include "engine/Color.hpp"
#include "planets/ShapeSettings.hpp"


enum ObsFlag
{
	RESOLUTION = 0,
	COLOR,
	RADIUS
};


class ObserverHud
{
public:
	ObserverHud();

	std::vector<ObsFlag> checkHud(int& resolution, Color& color, std::shared_ptr<ShapeSettings>& shapeSettings);

private:

};
