#pragma once

#include "engine/Color.hpp"
#include "opengl/Mesh.hpp"
#include "opengl/Shader.h"
#include <memory>

class BackgroundGradient
{
public:
	BackgroundGradient();

	void Draw(float viewportHeight);

private:
	Mesh _mesh;
	std::shared_ptr<Shader> _shader;
};