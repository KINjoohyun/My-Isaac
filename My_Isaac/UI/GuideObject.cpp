#include "stdafx.h"
#include "GuideObject.h"
#include "ResourceMgr.h"

void GuideObject::Init()
{
	SpriteGameObject::Init();

	text.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
	text.setCharacterSize(17);
}
void GuideObject::Reset()
{
	SpriteGameObject::Reset();

	timer = 0.0f;
	sprite.setScale(3.0f, 2.0f);
	text.setString(name);
	Utils::SetOrigin(text, origin);
}
void GuideObject::Update(float dt)
{
	SpriteGameObject::Update(dt);

	if (timer < duration) timer += dt;
	else
	{
		SetActive(false);
		timer = 0.0f;
	}
}
void GuideObject::Draw(sf::RenderWindow& window)
{
	SpriteGameObject::Draw(window);
	window.draw(text);
}

void GuideObject::SetPosition(const sf::Vector2f& position)
{
	SpriteGameObject::SetPosition(position);
	text.setPosition(position);
}
void GuideObject::SetPosition(float x, float y)
{
	SpriteGameObject::SetPosition(x, y);
	text.setPosition(x, y);
}

void GuideObject::SetOrigin(Origins origin)
{
	SpriteGameObject::SetOrigin(origin);
	Utils::SetOrigin(text, origin);
}

void GuideObject::SetString(const std::string& text)
{
	name = text;
	Utils::SetOrigin(this->text, origin);
	SetActive(true);
}