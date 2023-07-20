#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "ResourceMgr.h"

Monster::Monster(ObjType objtype, const std::string& textureId, const std::string& name)
	:RoomObject(textureId, name), objtype(objtype)
{
	switch (objtype)
	{
	case ObjType::AttackFly:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/AttackFly1.csv"));
		break;
	case ObjType::Pooter:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/AttackFly1.csv"));
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
	};
}
void Monster::Reset()
{
	RoomObject::Reset();

	switch (objtype)
	{
	case ObjType::AttackFly:
		animation.Play("AttackFly1");
		break;
	case ObjType::Pooter:
		animation.Play("AttackFly1");
		break;
	case ObjType::Sucker:
		animation.Play("Sucker1");
		break;
	}
}
void Monster::Update(float dt)
{
	RoomObject::Update(dt);
	animation.Update(dt);
	
	if (isChase)
	{
		sf::Vector2f dir = Utils::Normalize(player->GetPosition() - position);

		if (Utils::Distance(player->GetPosition(), position) < recognize)
		{
			SetFlipX(sprite, dir.x < 0.0f);
			SetPosition(position + dir * speed * dt);
		}
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