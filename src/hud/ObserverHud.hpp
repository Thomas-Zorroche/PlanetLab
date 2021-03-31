#pragma once

#include <vector>

enum ObsFlag
{
	RESOLUTION
};


class ObserverHud
{
public:
	ObserverHud();

	std::vector<ObsFlag> checkHud(int& resolution);

private:

};
