#include "stdafx.h"
#include "Monster.h"
#include "Player.h"

Monster::Monster(const std::string& textureId, const std::string& name)
	:RoomObject(textureId, name)
{

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
}
void Monster::Update(float dt)
{
	RoomObject::Update(dt);
	
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