#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Player;
class RoomObject;

class Tear : public SpriteGameObject
{
protected:
	Player* player;

	AnimationController animation;

	float speed = 0.0f;
	float range = 300.0f;
	int damage = 0;
	sf::Vector2f direction;

	sf::FloatRect wall;
	float wallTop = 0.0f;
	float wallBottom = 0.0f;
	float wallLeft = 0.0f;
	float wallRight = 0.0f;

	const std::list<RoomObject*>* hitablelist;

	sf::Sound shootsound;

public:
	ObjectPool<Tear>* pool;

	Tear(const std::string& textureId = "", const std::string& name = "tear");
	virtual ~Tear() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void Shoot(const sf::Vector2f& position, const sf::Vector2f& direction, float speed, int damage);
	void SetWall(const sf::FloatRect& wall);
	void SetPlayer(Player* player);
	void SetHitlist(const std::list<RoomObject*>* list);
};