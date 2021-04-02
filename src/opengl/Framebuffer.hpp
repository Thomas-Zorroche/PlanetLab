#pragma once

class Framebuffer
{
public:
	Framebuffer(float width, float height);

	unsigned int id() const { return _id; }

private:
	unsigned int _id;
};