#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

Boss::Boss(ObjType objtype, const std::string& textureId, const std::string& name)
	:Monster(objtype, textureId, name)
{
	switch (objtype)
	{
	case ObjType::DukeOfFlies:
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Duke_Of_Flies1.csv"));
		animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Duke_Of_Flies2.csv"));
		break;
	}

	animation.SetTarget(&sprite);
}

void Boss::Init()
{
	Monster::Init();
}
void Boss::Reset()
{
	Monster::Reset();

	p1Timer = 0.0f;
	p2Timer = 0.0f;
	p3Timer = 0.0f;
	animation.Play("Idle");
}
void Boss::Update(float dt)
{
	Monster::Update(dt);

	if (Pattern1 != nullptr && animation.GetCurrentClipId() == "Idle")
	{
		if (p1Timer < p1Cooltime)
		{
			p1Timer += dt;
			return;
		}
		if (Pattern1())
		{
			p1Timer = 0.0f;
			animation.Play("Pattern1");
			animation.PlayQueue("Idle");
		}
	}
	if (Pattern2 != nullptr && animation.GetCurrentClipId() == "Idle")
	{
		if (p2Timer < p2Cooltime)
		{
			p2Timer += dt;
			return;
		}
		if (Pattern2())
		{
			p2Timer = 0.0f;
			animation.Play("Pattern2");
			animation.PlayQueue("Idle");
		}
		
	}
	if (Pattern3 != nullptr && animation.GetCurrentClipId() == "Idle")
	{
		if (p3Timer < p3Cooltime)
		{
			p3Timer += dt;
			return;
		}
		if (Pattern3())
		{
			p3Timer = 0.0f;
			animation.Play("Pattern3");
			animation.PlayQueue("Idle");
		}
	}
}
void Boss::Release()
{
	Monster::Release();
}

void Boss::SetCooltime(float p1Cooltime, float p2Cooltime, float p3Cooltime)
{
	this->p1Cooltime = p1Cooltime;
	this->p2Cooltime = p2Cooltime;
	this->p3Cooltime = p3Cooltime;
}