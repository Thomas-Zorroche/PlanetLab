#include "SpriteSheet.hpp"
#include "engine/ResourceManager.hpp"

namespace editor
{

SpriteSheet::SpriteSheet(const std::string& texturePath, int spriteCount)
	: _texture(ResourceManager::Get().LoadTexture(texturePath)),
	_spriteCount(spriteCount)
{
	_spriteWidth = _texture.Width() / (float)spriteCount;
	_uv1 = glm::vec2(0, 1);
	_uv2 = glm::vec2(_spriteWidth, 0);
}

void SpriteSheet::NextSprite()
{
	_uv1.x += _spriteWidth;
	_uv2.x += _spriteWidth;

	if ((_uv1.x > _texture.Width() - _spriteWidth) ||
		(_uv2.x > _texture.Width()))
	{
		_uv1.x = 0;
		_uv2.x = _spriteWidth;
	}
}

glm::vec2 SpriteSheet::GetUV1() const
{
	return glm::vec2(_uv1.x / _texture.Width(), _uv1.y);
}

glm::vec2 SpriteSheet::GetUV2() const
{
	return glm::vec2(_uv2.x / _texture.Width(), _uv2.y);
}

}   // ns editor