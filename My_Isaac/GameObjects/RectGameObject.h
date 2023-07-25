#pragma once
#include "GameObject.h"

class RectGameObject : public GameObject
{
protected:
	
public:
	sf::RectangleShape rect;

	RectGameObject(const std::string name = "") : GameObject(name) {}
	virtual ~RectGameObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	std::function<void()> OnDebug;
};