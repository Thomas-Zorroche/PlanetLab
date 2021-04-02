#include "ShapeSettings.hpp"

#include "noise/NoiseSettings.hpp"


ShapeSettings::ShapeSettings(float radius)
	: _planetRadius(radius), _noiseSettings(std::make_shared<NoiseSettings>()) {}

float& ShapeSettings::planetRadius() 
{ 
	return _planetRadius; 
}
float ShapeSettings::planetRadius() const 
{ 
	return _planetRadius; 
}

std::shared_ptr<NoiseSettings> ShapeSettings::noiseSettings()
{ 
	return _noiseSettings; 
}