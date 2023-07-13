#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

class Player : public SpriteGameObject
{
public:

protected:
	AnimationController animation;
	sf::Vector2f velocity;

	float accel = 500.f;
	float speed = 500.f;
	float JumpForce = -250.f;
	float gravity = 500.f;

	bool flipX = false;
	bool isGround = true;

public:
	Player(const std::string& textureId = "", const std::string& n = "")
		: SpriteGameObject(textureId, n) {}
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool flip);
	
};

