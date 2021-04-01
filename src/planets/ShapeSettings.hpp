#pragma once

class ShapeSettings
{
public:
	ShapeSettings(float radius = 1.0f)
		: _planetRadius(radius) {}

private:
	float _planetRadius;
};