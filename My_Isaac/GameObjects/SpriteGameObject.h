#pragma once
#include "GameObject.h"

class SpriteGameObject : public GameObject
{
protected:
	bool flipX = false;
public:
	std::string textureId;
	sf::Sprite sprite;
	sf::RectangleShape col;

	SpriteGameObject(const std::string textureId = "", const std::string name = "") : GameObject(name), textureId(textureId) {}
	virtual ~SpriteGameObject() override { Release(); }

	virtual void Init() override {}
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override {}

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void SetFlipX(sf::Sprite& sprite, bool flip);

	std::function<void()> OnDebug;
};