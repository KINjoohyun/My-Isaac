#pragma once
#include "GameObject.h"

class Collider : public GameObject
{
protected:
	sf::RectangleShape hitbox;
public:
	sf::FloatRect col;

	Collider(const std::string& name = "");
	virtual ~Collider() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	void SetCol(const sf::FloatRect& col);
};