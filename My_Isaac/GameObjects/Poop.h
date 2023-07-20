#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

class Poop : public SpriteGameObject
{
protected:
	ObjType objtype = ObjType::Poop;

	int hp = 0;
	int maxHp = 4;
public:
	Poop(const std::string& textureId = "", const std::string& name = "");
	virtual ~Poop() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() {}

	void OnHit(int damage);
};