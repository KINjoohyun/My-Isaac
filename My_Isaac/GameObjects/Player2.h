#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

enum class Look
{
	Left,
	Up,
	Down,
	Count,
	//Right,
};

class Player2 : public SpriteGameObject
{
protected:
	AnimationController animation;

	sf::Vector2f direction = {0.0f, -1.0f};
	float speed = 300.0f;

	bool flipX = false;

	Look look;
public:
	Player2(const std::string textureId = "", const std::string name = "");
	virtual ~Player2() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool flip);
};