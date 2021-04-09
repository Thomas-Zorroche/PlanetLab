#pragma once

#include <vector>
#include <memory>
#include <string>
#include "glm/glm.hpp"

class StaticMesh;
class Skybox;
class Planet;

class Scene
{
public:
	Scene();
	~Scene();

	void Init();

	void Draw(bool wireframe = false);

	std::shared_ptr<StaticMesh>& StaticMeshPtr(int index) { return _staticMeshes[index]; }

	void Free();

	std::shared_ptr<Planet> planet() { return _planet; }

private:
	void AddStaticMesh(const std::shared_ptr<StaticMesh>& mesh);

private:
	std::vector<std::shared_ptr<StaticMesh> > _staticMeshes;
	std::shared_ptr<Skybox> _skybox;
	std::shared_ptr<Planet> _planet;

	unsigned int _staticMeshesCount = 0;
};

