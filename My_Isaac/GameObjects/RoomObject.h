#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

class Player;

class RoomObject : public SpriteGameObject
{
protected:
	int hp = 0;
	int maxHp = 0;

	Player* player;

public:
	RoomObject(const std::string& textureId = "", const std::string& name = "");
	virtual ~RoomObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() {}

	std::function<void(int damage)> OnHit; // Tear客 面倒
	std::function<void()> OnBump; // Player客 面倒

	void SetPlayer(Player* player);
	void SetMaxHp(int maxHp);
	void OnDamage(int damage);
	bool IsBump();
};