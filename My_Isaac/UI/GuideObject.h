#pragma once
#include "SpriteGameObject.h"

class GuideObject : public SpriteGameObject
{
protected:
	sf::Text text;
	float timer = 0.0f;
	float duration = 1.5f;

public:
	GuideObject(const std::string& textureId = "", const std::string& name = "") : SpriteGameObject(textureId, name) {}
	virtual ~GuideObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);

	void SetString(const std::string& text);
};