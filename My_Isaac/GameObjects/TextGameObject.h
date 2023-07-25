#pragma once
#include "GameObject.h"

class TextGameObject : public GameObject
{
protected:

public:
	std::string fontId;
	sf::Text text;

	TextGameObject(const std::string fontId = "", const std::string name = "");
	virtual ~TextGameObject() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() {}

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	std::function<void()> action;
};