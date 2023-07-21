#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

Monster::Monster(ObjType objtype, const std::string& textureId, const std::string& name)
	:RoomObject(textureId, name), objtype(objtype)
{
	switch (objtype)
	{
	case ObjType::AttackFly:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/AttackFly1.csv"));
		break;
	case ObjType::Pooter:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Pooter1.csv"));
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Pooter2.csv"));
		break;
	case ObjType::Sucker:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Sucker1.csv"));
		break;
	}

	animation.SetTarget(&sprite);
}

void Monster::Init()
{
	RoomObject::Init();

	OnHit = [this](int dmg)
	{
		OnDamage(dmg);
		hitedTimer = 0.0f;
		SetPosition(GetPosition() - direction * 10.0f);
	};

	sprite.setScale(2.0f, 2.0f);
}
void Monster::Reset()
{
	RoomObject::Reset();

	animation.Play("MonsterIdle");
	SetOrigin(origin);
	sprite.setColor(sf::Color::White);
	hitedTimer = hitedDuration;
}
void Monster::Update(float dt)
{
	RoomObject::Update(dt);
	animation.Update(dt);

	if (hitedTimer < hitedDuration)
	{
		hitedTimer += dt;
		sprite.setColor({176, 16, 48});
	}
	else if (sprite.getColor() != sf::Color::White)
	{
		sprite.setColor(sf::Color::White);
	}


	if (isChase)
	{
		direction = Utils::Normalize(player->GetPosition() - position);
	}

	if (Utils::Distance(player->GetPosition(), position) < recognize)
	{
		SetFlipX(sprite, direction.x < 0.0f);
		SetPosition(position + direction * speed * dt);
	}
}

void Monster::SetMonster(int damage, float speed, int maxHp, float recognize, bool isChase)
{
	this->damage = damage;
	this->speed = speed;
	SetMaxHp(maxHp);
	this->recognize = recognize;
	this->isChase = isChase;
}
const sf::Vector2f& Monster::GetDirection()
{
	return direction;
}
const int& Monster::GetDamage()
{
	return damage;
}