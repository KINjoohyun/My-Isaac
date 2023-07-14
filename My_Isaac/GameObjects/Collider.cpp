#include "stdafx.h"
#include "Collider.h"

Collider::Collider(const std::string& name) : GameObject(name)
{

}
Collider::~Collider()
{
	//Release();
}

void Collider::Init()
{
}
void Collider::Reset()
{
}
void Collider::Update(float dt)
{
}
void Collider::Draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}
void Collider::Release()
{
}

void Collider::SetCol(const sf::FloatRect& col)
{
	this->col = col;
}