#include "stdafx.h"
#include "VertexArrayGameObject.h"
#include "ResourceMgr.h"

VertexArrayGameObject::VertexArrayGameObject(const std::string& textureId, const std::string& name)
	:GameObject(name), textureId(textureId), texture(nullptr)
{

}
VertexArrayGameObject::~VertexArrayGameObject()
{

}

void VertexArrayGameObject::Init()
{
	
}
void VertexArrayGameObject::Reset()
{
	texture = RESOURCE_MGR.GetTexture(textureId);
}
void VertexArrayGameObject::Update(float dt)
{
	
}
void VertexArrayGameObject::Draw(sf::RenderWindow& window)
{
	window.draw(vertexArray, texture);
}
void VertexArrayGameObject::Release()
{
	
}

void VertexArrayGameObject::SetPosition(const sf::Vector2f& position)
{
	sf::Vector2f diff = position - this->position;
	GameObject::SetPosition(position);

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff;
	}
}
void VertexArrayGameObject::SetPosition(float x, float y)
{
	sf::Vector2f diff = { x - this->position.x, y - this->position.y };
	GameObject::SetPosition(x, y);

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += diff;
	}
}

void VertexArrayGameObject::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	sf::Vector2f prevOrigin = originPos;

	originPos = { vertexArray.getBounds().width, vertexArray.getBounds().height };
	originPos.x *= (int)origin % 3 * 0.5f;
	originPos.y *= (int)origin / 3 * 0.5f;

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += prevOrigin - originPos;
	}
	
}
void VertexArrayGameObject::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);

	sf::Vector2f prevOrigin = originPos;
	originPos.x = x;
	originPos.y = y;

	for (int i = 0; i < vertexArray.getVertexCount(); i++)
	{
		vertexArray[i].position += prevOrigin - originPos;
	}
}