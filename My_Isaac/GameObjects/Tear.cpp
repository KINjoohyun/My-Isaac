#include "stdafx.h"
#include "Tear.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"

Tear::Tear(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/TearShooting.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/TearSplash.csv"));
	animation.SetTarget(&sprite);
}
Tear::~Tear()
{

}

void Tear::Init()
{
	SpriteGameObject::Init();

	sprite.setScale(1.5f, 1.5f);
	SetOrigin(Origins::C);
}
void Tear::Reset()
{
	SpriteGameObject::Reset();

	SetOrigin(origin);
	SetPosition(0.0f, 0.0f);
	direction = { 0.0f, 0.0f };
	speed = 0.0f;
	damage = 0;

	animation.Play("TearShooting");
}
void Tear::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	SetPosition(position + direction * speed * dt);

	range -= speed * dt;
	timer += dt;
	
	if ((range < 0.0f) && animation.GetCurrentClipId() == "TearShooting")
	{
		animation.Play("TearSplash");
		SetOrigin(Origins::C);
		direction = {0.0f, 0.0f};
	}
	if (timer >= duration) // 충돌 조건 추가 필요
	{
		pool->Return(this);
		SCENE_MGR.GetCurrentScene()->RemoveGO(this);
		return;
	}
}

void Tear::Shoot(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, float damage)
{
	SetPosition(position);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
}