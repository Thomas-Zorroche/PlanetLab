#include "NoiseLayer.hpp"

namespace Ceres
{

NoiseLayer::NoiseLayer(int id, const std::shared_ptr<NoiseSettings>& settings)
	: _id(id), _noiseSettings(settings), _enabled(true)
{}
}