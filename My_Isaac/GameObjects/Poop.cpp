#include "stdafx.h"
#include "Poop.h"
#include "ResourceMgr.h"

Poop::Poop(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{

}

void Poop::Init()
{
	SpriteGameObject::Init();
}
void Poop::Reset()
{
	SpriteGameObject::Reset();

	hp = maxHp;
}
void Poop::Update(float dt)
{
	SpriteGameObject::Update(dt);
}

void Poop::OnHit(int damage)
{
	hp = std::max(0, hp - damage);

	// test
	if (hp == 0)
	{
		SetActive(false);
	}
}