#pragma once

#include "engine/Color.hpp"
#include <memory>

class Color;
class Shader;

class ColorSettings
{
public:
	ColorSettings(const Color& color = Color());
	
	const Color& color() const;
	Color& color();

	float GetOceanDepth() const { return _oceanDepth; }
	float& GetOceanDepth() { return _oceanDepth; }

	Color GetOceanColor() const { return _oceanColor; }
	Color& GetOceanColor() { return _oceanColor; }

	void SendUniforms(std::shared_ptr<Shader>& shader);

private:
	Color _colorPlanet;

	// Ocean settings
	float _oceanDepth = 0.0f;
	Color _oceanColor = Color(0, 0, 1);
};