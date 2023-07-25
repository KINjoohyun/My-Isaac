#pragma once
#include "SpriteGameObject.h"

class UIImageButton : public SpriteGameObject
{
protected:
	bool isHover = false;
public:
	UIImageButton(const std::string& textureId = "", const std::string& name = "");
	virtual ~UIImageButton() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override { Release(); }

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;
};