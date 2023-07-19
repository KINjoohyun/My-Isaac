#pragma once
#include "GameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Tear.h"

class Player : public GameObject
{
protected:
	AnimationController bodyAnimation;
	AnimationController headAnimation;

	sf::Vector2f direction = {0.0f, -1.0f};
	float speed = 300.0f;
	bool flipX = false;

	ObjectPool<Tear> poolTears;

public:
	sf::Sprite head;
	sf::Sprite body;

	Player(const std::string name = "");
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override { poolTears.Release(); }

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	bool GetFlipX() const;
	void SetFlipX(sf::Sprite& sprite, bool flip);

	void TearShoot(const sf::Vector2f& direction);
};