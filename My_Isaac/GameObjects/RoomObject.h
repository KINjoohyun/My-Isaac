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

	sf::FloatRect wall;
	float wallTop = 0.0f;
	float wallBottom = 0.0f;
	float wallLeft = 0.0f;
	float wallRight = 0.0f;

public:

	RoomObject(const std::string& textureId = "", const std::string& name = "") : SpriteGameObject(textureId, name) {}
	virtual ~RoomObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	std::function<void(int damage)> OnHit; // Tear�� �浹
	std::function<void()> OnBump; // Player�� �浹
	std::function<void()> OnDie; //��� ��

	void SetPlayer(Player* player);
	void SetMaxHp(int maxHp);
	virtual void OnDamage(int damage);
	bool IsBump();
	void SetWall(const sf::FloatRect& wall);

	const int GetHp() const;
};