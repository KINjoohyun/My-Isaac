#include "stdafx.h"
#include "RoomObject.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "Player.h"

RoomObject::RoomObject(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{

}

void RoomObject::Init()
{
	SpriteGameObject::Init();
}
void RoomObject::Reset()
{
	SpriteGameObject::Reset();

	hp = maxHp;
}
void RoomObject::Update(float dt)
{
	SpriteGameObject::Update(dt);

	if (!wall.contains(position))
	{
		SetPosition(Utils::Clamp(position, { wallLeft, wallTop }, { wallRight, wallBottom }));
	}

	if (IsBump())
	{
		if (OnBump != nullptr)
		{
			OnBump();
		}
	}
}

void RoomObject::SetPlayer(Player* player)
{
	this->player = player;
}
void RoomObject::SetMaxHp(int maxHp)
{
	this->maxHp = maxHp;
}
void RoomObject::OnDamage(int damage)
{
	hp = std::max(0, hp - damage);

	if (hp == 0)
	{
		if (OnDie != nullptr)
		{
			OnDie();
		}
		else
		{
			SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
			scene->RemoveRGO(this);
		}
	}
}
bool RoomObject::IsBump()
{
	if (player == nullptr)	return false;
	return sprite.getGlobalBounds().intersects(player->body.getGlobalBounds());
}
void RoomObject::SetWall(const sf::FloatRect& wall)
{
	this->wall = wall;

	wallTop = wall.top;
	wallBottom = wall.top + wall.height;
	wallLeft = wall.left;
	wallRight = wall.left + wall.width;
}

const int RoomObject::GetHp() const
{
	return hp;
}