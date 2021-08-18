#pragma once

#include "editor/ui/gizmo/Gizmo.hpp"


namespace Ceres 
{
class Planet;
}

namespace PlanetLab
{

class Application;
class Skybox;

class Scene
{
public:
	Scene();
	~Scene() = default;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Draw();

	void Free();

	std::shared_ptr<Ceres::Planet> planet() { return _planet; }

private:
	std::shared_ptr<Ceres::Planet> _planet = nullptr;

	std::shared_ptr<Skybox> _skybox = nullptr;

	Gizmo _gizmo = Gizmo(2, 0.005);
};

} // ns Editor

