#pragma once

#include "engine/Color.hpp"

class Framebuffer
{
public:
	Framebuffer(float width = 64.0f, float height = 64.0f);

	unsigned int id() const { return _id; }
	void resize(float width, float height);

	void bind(float viewportWidth, float viewportHeight);

	void unbind();

	void free();

private:
	unsigned int _id;
	unsigned int _textureID;
	unsigned int _rboID;
};