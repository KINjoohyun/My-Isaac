#include "stdafx.h"
#include "EffectObject.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"

EffectObject::EffectObject(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/TearSplash.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BloodSplash.csv"));
	animation.SetTarget(&sprite);
}

void EffectObject::Init()
{
	SpriteGameObject::Init();

	sprite.setScale(1.5f, 1.5f);
}
void EffectObject::Reset()
{
	SpriteGameObject::Reset();

	SetOrigin(Origins::C);
	SetPosition(0.0f, 0.0f);
	timer = 0.0f;
	animation.Stop();

	splashsound.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/Splash" + std::to_string(Utils::RandomRange(0, 2)) + ".ogg"));
	splashsound.play();
}
void EffectObject::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	if (!animation.IsPlaying())
	{
		animation.Play(anim);
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

void EffectObject::SetAnimation(const std::string& anim)
{
	this->anim = anim;
}