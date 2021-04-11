#pragma once

#include "opengl/Texture.h"
#include "glm/glm.hpp"

class SpriteSheet
{
public:
	SpriteSheet(const std::string& texturePath, int spriteCount);

	std::uint32_t GetId() const { return _texture.Id(); }

	void NextSprite();

	glm::vec2 GetUV1() const;
	glm::vec2 GetUV2() const;

private:
	Texture _texture;

	int _spriteCount = 0;
	float _spriteWidth = 0.0f;

	glm::vec2 _uv1;
	glm::vec2 _uv2;
};