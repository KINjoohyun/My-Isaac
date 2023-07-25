#include "stdafx.h"
#include "RectGameObject.h"

void RectGameObject::Init()
{
	
}
void RectGameObject::Reset()
{
	
}
void RectGameObject::Update(float dt)
{
	if (OnDebug != nullptr) OnDebug();
}
void RectGameObject::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
}
void RectGameObject::Release()
{

}

void RectGameObject::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	rect.setPosition(position);
}
void RectGameObject::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	rect.setPosition(x, y);
}

void RectGameObject::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(rect, origin);
	}
}
void RectGameObject::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	rect.setOrigin(x, y);
}