#pragma once

#include <memory>

class Skybox;
class Planet;
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

	std::shared_ptr<Planet> planet() { return _planet; }

private:
	std::shared_ptr<Skybox> _skybox = nullptr;
	std::shared_ptr<Planet> _planet = nullptr;
	std::shared_ptr<BackgroundGradient> _backgroundPlane = nullptr;
};

