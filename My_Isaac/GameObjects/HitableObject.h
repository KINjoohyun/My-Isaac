#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

class Player;

class HitableObject : public SpriteGameObject
{
protected:
	ObjType objtype = ObjType::Poop;

	int hp = 0;
	int maxHp = 0;

	Player* player;

public:
	HitableObject(const std::string& textureId = "", const std::string& name = "");
	virtual ~HitableObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() {}

	std::function<void(int damage)> OnHit; // Tear客 面倒

	void SetPlayer(Player* player);
	void SetMaxHp(int maxHp);
	void OnDamage(int damage);
	bool OnBump(); // Player客 面倒
};