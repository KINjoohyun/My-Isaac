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
}
void Poop::Update(float dt)
{
	SpriteGameObject::Update(dt);
}