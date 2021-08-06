#pragma once

#include "engine/Color.hpp"
#include "engine/opengl/Mesh.hpp"
#include "engine/opengl/Shader.h"

namespace PlanetLab
{

class BackgroundGradient
{
public:
	BackgroundGradient();

	void Draw(float viewportHeight);

private:
	Mesh _mesh;
	std::shared_ptr<Shader> _shader;
};

} // ns editor