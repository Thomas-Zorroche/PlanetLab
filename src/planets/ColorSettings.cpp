#include "ColorSettings.hpp"

ColorSettings::ColorSettings(const Color& color)
	: _colorPlanet(color) {}

const Color& ColorSettings::planetColor() const 
{ 
	return _colorPlanet; 
}

Color& ColorSettings::planetColor() 
{ 
	return _colorPlanet; 
}