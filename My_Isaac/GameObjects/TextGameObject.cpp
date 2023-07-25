#include "stdafx.h"
#include "TextGameObject.h"
#include "ResourceMgr.h"

TextGameObject::TextGameObject(const std::string fontId, const std::string name)
	:GameObject(name), fontId(fontId)
{

}
TextGameObject::~TextGameObject()
{

}

void TextGameObject::Init()
{

}
void TextGameObject::Reset()
{
	if (RESOURCE_MGR.GetFont(fontId) != nullptr)
	{
		text.setFont(*RESOURCE_MGR.GetFont(fontId));
	}
	SetOrigin(origin);
}
void TextGameObject::Update(float dt)
{
	if (action != nullptr)
	{
		action();
	}
}
void TextGameObject::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGameObject::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	text.setPosition(position);
}
void TextGameObject::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	text.setPosition(this->position);
}
void TextGameObject::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(text, origin);
	}
}
void TextGameObject::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	text.setOrigin(x, y);
}