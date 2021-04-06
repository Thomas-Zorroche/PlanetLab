#pragma once

#include "engine/Color.hpp"

class Color;

class ColorSettings
{
public:
	ColorSettings(const Color& color = Color());
	
	const Color& color() const;
	Color& color();

private:
	Color _colorPlanet;
};