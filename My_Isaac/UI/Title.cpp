#include "stdafx.h"
#include "Title.h"
#include "ResourceMgr.h"

Title::Title(const std::string& textureId, const std::string& name)
	: SpriteGameObject(textureId, name)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Title.csv"));
	animation.SetTarget(&sprite);
}

void Title::Init()
{
	SpriteGameObject::Init();

	SetOrigin(Origins::C);
}
void Title::Reset()
{
	animation.Play("Idle");

	SpriteGameObject::Reset();
}
void Title::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);
}