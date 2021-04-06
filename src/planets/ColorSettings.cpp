#include "ColorSettings.hpp"

ColorSettings::ColorSettings(const Color& color)
	: _colorPlanet(color) {}

const Color& ColorSettings::color() const 
{ 
	return _colorPlanet; 
}

Color& ColorSettings::color() 
{ 
	return _colorPlanet; 
}