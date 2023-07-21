#include "stdafx.h"
#include "Tear.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "Player.h"
#include "RoomObject.h"

Tear::Tear(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/TearShooting.csv"));
	animation.SetTarget(&sprite);
}

void Tear::Init()
{
	SpriteGameObject::Init();

	sprite.setScale(1.5f, 1.5f);
}
void Tear::Reset()
{
	SpriteGameObject::Reset();

	SetPosition(0.0f, 0.0f);
	range = 300.0f;
	direction = { 0.0f, 0.0f };
	damage = 0;

	animation.Play("TearShooting");
	SetOrigin(Origins::C);
}
void Tear::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	SetPosition(position + direction * speed * dt);

	range -= speed * dt;

	if (!wall.contains(position) || range < 0.0f)
	{
		SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
		player->TearSplash(position);

		pool->Return(this);
		scene->RemoveGO(this);
		return;
	}

	if (hitablelist != nullptr)
	{
		for (auto it : *hitablelist)
		{
			if (sprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds()) && isActive)
			{
				if (it->OnHit != nullptr)
				{
					it->OnHit(damage);
				}

				SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
				player->TearSplash(position);

				pool->Return(this);
				scene->RemoveGO(this);
				break;
			}
		}
	}
}

void Tear::Shoot(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, int damage)
{
	SetPosition(position);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
}
void Tear::SetWall(const sf::FloatRect& wall)
{
	this->wall = wall;

	wallTop = wall.top;
	wallBottom = wall.top + wall.height;
	wallLeft = wall.left;
	wallRight = wall.left + wall.width;
}
void Tear::SetPlayer(Player* player)
{
	this->player = player;
}
void Tear::SetHitlist(const std::list<RoomObject*>* list)
{
	this->hitablelist = list;
}