#include "NoiseLayer.hpp"

namespace Ceres
{

NoiseLayer::NoiseLayer(const std::shared_ptr<NoiseSettings>& settings)
	: _noiseSettings(settings), _enabled(true)
{}
}