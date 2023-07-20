#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "RectGameObject.h"
#include "SceneMgr.h"
#include "SceneGame.h"

Door::Door(const std::string& textureId, const sf::Vector2f& position, Look look)
	:SpriteGameObject(textureId), look(look)
{
	SetPosition(position);

	
}

void Door::Init()
{
	SpriteGameObject::Init();

	SetOrigin(Origins::C);
}
void Door::Reset()
{
	SpriteGameObject::Reset();

	switch (look)
	{
	case Look::Up:
		sprite.setRotation(0);
		break;
	case Look::Right:
		sprite.setRotation(90);
		break;
	case Look::Down:
		sprite.setRotation(180);
		break;
	case Look::Left:
		sprite.setRotation(270);
		break;
	}
}
void Door::Update(float dt)
{
	SpriteGameObject::Update(dt);

	if (sprite.getGlobalBounds().intersects(player->body.getGlobalBounds()) && isOpen)
	{
		Entrance();
	}
}

void Door::SetPlayer(Player* player)
{
	this->player = player;
}
void Door::SetDestination(const sf::Vector2f& destination)
{
	this->destination = destination;
}
void Door::Open()
{
	isOpen = true;
	textureId = "graphics/door_open.png";
	Reset();
}
void Door::Close()
{
	isOpen = false;
	textureId = "graphics/door_close.png";
	Reset();
}

void Door::Entrance()
{
	RectGameObject w;
	w.SetPosition(destination);
	w.rect.setSize({736.0f, 416.0f});
	w.SetOrigin(Origins::C);

	switch (look)
	{
	case Look::Up:
		player->SetPosition({ destination.x, destination.y + w.rect.getGlobalBounds().height/2 - 50.0f});
		break;
	case Look::Right:
		player->SetPosition({ destination.x - w.rect.getGlobalBounds().width / 2 + 50.0f, destination.y });
		break;
	case Look::Down:
		player->SetPosition({ destination.x, destination.y - w.rect.getGlobalBounds().height / 2 + 50.0f });
		break;
	case Look::Left:
		player->SetPosition({ destination.x + w.rect.getGlobalBounds().width / 2 - 50.0f, destination.y });
		break;
	}

	player->SetWall(sf::FloatRect(w.rect.getGlobalBounds()));
	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	scene->ViewSet(destination);
}