#pragma once

#include "engine/Color.hpp"

class Color;

class ColorSettings
{
public:
	ColorSettings(const Color& color = Color());
	
	const Color& planetColor() const;
	Color& planetColor();

private:
	Color _colorPlanet;
};