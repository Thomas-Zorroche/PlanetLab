#pragma once

class ShapeSettings
{
public:
	ShapeSettings(float radius = 1.0f)
		: _planetRadius(radius) {}

	float& planetRadius() { return _planetRadius; }
	float planetRadius() const { return _planetRadius; }

private:
	float _planetRadius;
};