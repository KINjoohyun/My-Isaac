#include "stdafx.h"
#include "Blood.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "Player.h"

Blood::Blood(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BloodShooting.csv"));
	animation.SetTarget(&sprite);
}

void Blood::Init()
{
	SpriteGameObject::Init();

	sprite.setScale(1.5f, 1.5f);
}
void Blood::Reset()
{
	SpriteGameObject::Reset();

	SetPosition(0.0f, 0.0f);
	range = 300.0f;
	direction = { 0.0f, 0.0f };
	speed = 0.0f;
	damage = 0;

	animation.Play("BloodShooting");
	SetOrigin(Origins::C);
}
void Blood::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	SetPosition(position + direction * speed * dt);

	range -= speed * dt;

	if (!wall.contains(position) || range < 0.0f)
	{
		player->Splash(position, "BloodSplash");

		SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
		// blood effect

		pool->Return(this);
		scene->RemoveGO(this);
		return;
	}

	if (sprite.getGlobalBounds().intersects(player->head.getGlobalBounds()) && isActive)
	{
		player->OnHit(damage);
		player->Splash(position, "BloodSplash");

		SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
		// blood effect

		pool->Return(this);
		scene->RemoveGO(this);
		return;
	}
}

void Blood::Shoot(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, int damage)
{
	SetPosition(position);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
}
void Blood::SetWall(const sf::FloatRect& wall)
{
	this->wall = wall;

	wallTop = wall.top;
	wallBottom = wall.top + wall.height;
	wallLeft = wall.left;
	wallRight = wall.left + wall.width;
}
void Blood::SetPlayer(Player* player)
{
	this->player = player;
}