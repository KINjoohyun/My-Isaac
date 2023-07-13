#pragma once
#include "SpriteGameObject.h"
class UIButton : public SpriteGameObject
{
protected:
	bool isHover = false;
public:
	sf::Text text;

	UIButton(const std::string& textureId = "", const std::string& name = "");
	virtual ~UIButton() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;
};