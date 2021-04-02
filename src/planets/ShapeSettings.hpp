#pragma once

#include <memory>

class NoiseSettings;

class ShapeSettings
{
public:
	ShapeSettings(float radius = 1.0f);
	float& planetRadius();
	float planetRadius() const;

	std::shared_ptr<NoiseSettings> noiseSettings();

private:
	float _planetRadius;
	std::shared_ptr<NoiseSettings> _noiseSettings;
};