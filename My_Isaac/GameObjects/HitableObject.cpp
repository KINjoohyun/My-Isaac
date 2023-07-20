#include "stdafx.h"
#include "HitableObject.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Player.h"

HitableObject::HitableObject(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{

}

void HitableObject::Init()
{
	SpriteGameObject::Init();
}
void HitableObject::Reset()
{
	SpriteGameObject::Reset();

	hp = maxHp;
}
void HitableObject::Update(float dt)
{
	SpriteGameObject::Update(dt);

	if (OnBump())
	{
		player->SetPosition(player->GetPosition() - Utils::Normalize(position - player->GetPosition()));
	}
}

void HitableObject::SetPlayer(Player* player)
{
	this->player = player;
}
void HitableObject::SetMaxHp(int maxHp)
{
	this->maxHp = maxHp;
}
void HitableObject::OnDamage(int damage)
{
	hp = std::max(0, hp - damage);

	if (hp == 0)
	{
		SCENE_MGR.GetCurrentScene()->RemoveGO(this);
	}
}
bool HitableObject::OnBump()
{
	if (player == nullptr)	return false;
	return sprite.getGlobalBounds().intersects(player->body.getGlobalBounds());
}