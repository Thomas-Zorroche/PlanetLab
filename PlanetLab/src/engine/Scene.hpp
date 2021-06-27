#pragma once

#include <memory>

#include "ui/gizmo/Gizmo.hpp"

namespace Ceres 
{
class Planet;
}


namespace PlanetLab
{

class BackgroundGradient;
class Application;

class Scene
{
public:
	Scene();
	~Scene() = default;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Draw(float viewportHeight);

	void Free();

	std::shared_ptr<Ceres::Planet> planet() { return _planet; }

private:
	std::shared_ptr<Ceres::Planet> _planet = nullptr;
	std::shared_ptr<BackgroundGradient> _backgroundPlane = nullptr;

	Gizmo _gizmo;
};

} // ns Editor

