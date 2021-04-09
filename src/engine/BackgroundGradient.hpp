#pragma once

#include "engine/Color.hpp"
#include "opengl/Mesh.hpp"
#include "opengl/Shader.h"
#include <memory>

class BackgroundGradient
{
public:
	BackgroundGradient();

	Color GetColot() const { return _color; }
	Color& GetColot() { return _color; }

	void Draw(float viewportHeight, const Color& backgroundColor);

private:
	Color _color = Color();
	Mesh _mesh;
	std::shared_ptr<Shader> _shader;
};