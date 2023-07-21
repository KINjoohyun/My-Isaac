#include "stdafx.h"
#include "SpriteGameObject.h"
#include "ResourceMgr.h"

SpriteGameObject::SpriteGameObject(const std::string textureId, const std::string name)
	:GameObject(name), textureId(textureId)
{

}
SpriteGameObject::~SpriteGameObject()
{

}

void SpriteGameObject::Init()
{
	
}
void SpriteGameObject::Reset()
{
	if (RESOURCE_MGR.GetTexture(textureId) != nullptr)
	{
		sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	}
	SetOrigin(origin);
}
void SpriteGameObject::Update(float dt)
{
	
}
void SpriteGameObject::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void SpriteGameObject::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	sprite.setPosition(position);
}
void SpriteGameObject::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	sprite.setPosition(this->position);
}

void SpriteGameObject::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(sprite, origin);
	}
}
void SpriteGameObject::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void SpriteGameObject::SetFlipX(sf::Sprite& sprite, bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flipX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}