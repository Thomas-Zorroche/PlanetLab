#include "ColorSettings.hpp"
#include "opengl/Shader.h"

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

void ColorSettings::SendUniforms(std::shared_ptr<Shader>& shader)
{
	shader->SetUniform1f("u_oceanDepth", _oceanDepth);
	shader->SetUniform3f("u_oceanColor", _oceanColor.vector());
}
