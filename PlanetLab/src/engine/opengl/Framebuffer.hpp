#pragma once

namespace PlanetLab
{

class Framebuffer
{
public:
	Framebuffer(float width = 64.0f, float height = 64.0f);

	unsigned int getId() const { return _id; }
	unsigned int getTextureId() const { return _textureID; }
	void resize(float width, float height);

	void bind(float viewportWidth, float viewportHeight);

	void unbind();

	void free();

private:
	unsigned int _id;
	unsigned int _textureID;
	unsigned int _rboID;
};

}