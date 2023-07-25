#pragma once
#include "GameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Tear.h"
#include "EffectObject.h"

class Player : public GameObject
{
protected:
	AnimationController bodyAnimation;
	AnimationController headAnimation;

	sf::Vector2f direction = {0.0f, -1.0f};
	float speed = 300.0f;
	bool flipX = false;

	ObjectPool<Tear> poolTears;
	ObjectPool<EffectObject> poolEffects;

	int maxLife = 3;
	int life = 0;
	int damage = 1;

	sf::FloatRect wall;
	float wallTop = 0.0f;
	float wallBottom = 0.0f;
	float wallLeft = 0.0f;
	float wallRight = 0.0f;

	float invincibleTimer = 0.0f;
	float invincibleDuration = 1.0f;

	float attackTimer = 0.0f;
	float attackDuration = 0.3f;

	bool isAlive = true;

public:
	sf::Sprite head;
	sf::RectangleShape headCol;
	sf::Sprite body;
	sf::RectangleShape bodyCol;

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

	bool GetFlipX() const;
	void SetFlipX(sf::Sprite& sprite, bool flip);

	void TearShoot(const sf::Vector2f& direction);
	void Splash(const sf::Vector2f& tearPos, const std::string& anim);
	void OnHit(int damage);
	void SetWall(const sf::FloatRect& wall);

	int GetMaxLife() const;
	int GetLife() const;

	void IncreaseLife(int life);
	void ChangeSpeed(float value);

	std::function<void()> OnDebug;
};