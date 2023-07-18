#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Tear : public SpriteGameObject
{
protected:
	AnimationController animation;

	float speed = 0.0f;
	float range = 300.0f;
	int damage = 0;
	sf::Vector2f direction;

public:
	ObjectPool<Tear>* pool;

	Tear(const std::string& textureId = "", const std::string& name = "");
	virtual ~Tear() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override { }

	void Shoot(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, float damage);
	void SetWall();
};