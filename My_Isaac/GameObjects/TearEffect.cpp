#include "stdafx.h"
#include "TearEffect.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"

TearEffect::TearEffect(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/TearSplash.csv"));
	animation.SetTarget(&sprite);
}

void TearEffect::Init()
{
	SpriteGameObject::Init();

	sprite.setScale(1.5f, 1.5f);
}
void TearEffect::Reset()
{
	SpriteGameObject::Reset();

	SetOrigin(Origins::C);
	SetPosition(0.0f, 0.0f);
	timer = 0.0f;
	animation.Stop();
}
void TearEffect::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	if (!animation.IsPlaying())
	{
		animation.Play("TearSplash");
		SetOrigin(Origins::C);
	}

	timer += dt;
	if (timer >= duration)
	{
		timer = 0.0f;
		pool->Return(this);
		SCENE_MGR.GetCurrentScene()->RemoveGO(this);
	}
}