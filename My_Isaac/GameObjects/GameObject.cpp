#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(const std::string name)
	:name(name)
{

}
GameObject::~GameObject()
{
	Release();
}

bool GameObject::GetActive() const
{
	return isActive;
}
void GameObject::SetActive(bool active)
{
	isActive = active;
}
std::string GameObject::GetName()
{
	return this->name;
}
void GameObject::SetName(const std::string& name)
{
	this->name = name;
}
sf::Vector2f GameObject::GetPosition()
{
	return this->position;
}
void GameObject::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
}
void GameObject::SetPosition(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
}

void GameObject::SetOrigin(Origins origin)
{
	this->origin = origin;
}
void GameObject::SetOrigin(float x, float y)
{
	this->origin = Origins::CUSTOM;
}