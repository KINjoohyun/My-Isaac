#pragma once
#include "GameObject.h"

class SpriteGameObject : public GameObject
{
protected:

public:
	std::string textureId;
	sf::Sprite sprite;

	SpriteGameObject(const std::string textureId = "", const std::string name = "");
	virtual ~SpriteGameObject() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() {}

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);


};