#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "RectGameObject.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "MiniMap.h"

Door::Door(const std::string& textureId, Look look, const std::tuple<int, int>& currRoom)
	:SpriteGameObject(textureId), look(look)
{
	this->currRoom = currRoom;
}

void Door::Init()
{
	SpriteGameObject::Init();

	SetOrigin(Origins::BC);
}
void Door::Reset()
{
	SpriteGameObject::Reset();

	switch (look)
	{
	case Look::Up:
		sprite.setRotation(0);
		col.setRotation(0);
		SetPosition(wallLeft + wall.width * 0.5f, wallTop);
		break;
	case Look::Right:
		sprite.setRotation(90);
		col.setRotation(90);
		SetPosition(wallRight, wallTop + wall.height * 0.5f);
		break;
	case Look::Down:
		sprite.setRotation(180);
		col.setRotation(180);
		SetPosition(wallLeft + wall.width * 0.5f, wallBottom);
		break;
	case Look::Left:
		sprite.setRotation(270);
		col.setRotation(270);
		SetPosition(wallLeft, wallTop + wall.height * 0.5f);
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
void Door::SetDestination(const sf::Vector2f& destination, const std::tuple<int, int>& nextRoom)
{
	this->destination = destination;
	this->nextRoom = nextRoom;
}
void Door::SetWall(const sf::FloatRect& wall)
{
	this->wall = wall;

	wallTop = wall.top;
	wallBottom = wall.top + wall.height;
	wallLeft = wall.left;
	wallRight = wall.left + wall.width;
}
void Door::Open()
{
	isOpen = true;
	sprite.setTextureRect({0, 0, 100, 80});
	Reset();
}
void Door::Close()
{
	isOpen = false;
	sprite.setTextureRect({ 100, 0, 100, 80 });
	Reset();
}

void Door::Entrance()
{
	RectGameObject w;
	w.SetPosition(destination);
	w.rect.setSize({ wall.width, wall.height });
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
	scene->stage1[std::get<0>(currRoom)][std::get<1>(currRoom)].isPassed = true;
	scene->stage1[std::get<0>(currRoom)][std::get<1>(currRoom)].isHear = false;
	scene->stage1[std::get<0>(nextRoom)][std::get<1>(nextRoom)].isHear = true;
	scene->minimap->RenewMiniMap();
}