 #pragma once

#include "engine/ResourceManager.hpp"
#include "opengl/Mesh.hpp"
#include "common.hpp"
#include "engine/Material.hpp"
#include "opengl/Shader.h"
#include "engine/Renderer.hpp"

namespace PlanetLab
{

class Skybox
{
public:
	Skybox(const std::vector<std::string>& faces, bool& display);

	void GenerateMesh();


	void Draw();

private:
	void SendUniforms();

	std::vector<std::string> _faces;

	unsigned int _id;

	std::shared_ptr<Shader> _shader;

	std::shared_ptr<Mesh> _mesh;

	static const float _SkyboxVertices[];

	bool& _displayed;
};

}	