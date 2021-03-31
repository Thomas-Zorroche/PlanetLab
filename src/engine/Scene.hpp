#pragma once

#include <vector>
#include <memory>
#include <string>
#include "glm/glm.hpp"

class StaticMesh;
class Skybox;

class Scene
{
public:
	Scene();
	~Scene();

	void Init();

	void Draw();

	std::shared_ptr<StaticMesh>& StaticMeshPtr(int index) { return _staticMeshes[index]; }

	void Free();

private:
	void AddStaticMesh(const std::shared_ptr<StaticMesh>& mesh);

private:
	std::vector<std::shared_ptr<StaticMesh> > _staticMeshes;
	std::shared_ptr<Skybox> _skybox;

	unsigned int _staticMeshesCount = 0;
};

