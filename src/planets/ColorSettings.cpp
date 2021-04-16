#include "ColorSettings.hpp"
#include "opengl/Shader.h"
#include <iostream> //TODO remove this

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
	// Ocean
	if (_useOceanColor)
	{
		shader->SetUniform1f("u_oceanDepth", _oceanDepth);
		shader->SetUniform3f("u_oceanColor", _oceanColor.vector());
	}
	else
	{
		shader->SetUniform1f("u_oceanDepth", -1); // Force to use only Landmass colors
	}

	// Landmass
	if (_useLandmassRamp)
	{
		shader->SetUniform1i("u_colorStepCount", _gradient.getMarks().size());
		std::size_t index = 0;
		for (auto it = _gradient.getMarks().begin(); it != _gradient.getMarks().end(); ++it)
		{
			std::string nameColor = "u_colors[" + std::to_string(index) + "]";
			std::string nameStep = "u_steps[" + std::to_string(index) + "]";
			auto markPtr = *it;
			shader->SetUniform3f(nameColor, markPtr->colorVector());
			shader->SetUniform1f(nameStep, markPtr->position);
			index++;
		}
	}
	else
	{
		shader->SetUniform1i("u_colorStepCount", 0);
	}

}

void ColorSettings::SetRandomColors(std::uint32_t seed)
{
	_useLandmassRamp = true;
	_useOceanColor = true;
	_gradient.getMarks().clear();

	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> random(0, 1);

	for (std::size_t i = 0; i < 4; i++)
	{
		auto pos = random(generator);
		ImColor color(
			random(generator),
			random(generator),
			random(generator)
		);
		_gradient.addMark(pos, color);
	}

	_oceanColor = Color(
		random(generator),
		random(generator),
		random(generator)
	);
	_oceanDepth = 9;
}

