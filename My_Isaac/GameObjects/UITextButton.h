#pragma once
#include "TextGameObject.h"

class UITextButton : public TextGameObject
{
protected:
	bool isHover = false;
public:
	UITextButton(const std::string& fontId = "", const std::string& name = "");
	virtual ~UITextButton() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override;

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	void SetText(const std::string& text = "", int size = 0, sf::Color textColor = sf::Color::White, float outlineSize = 1.0f, sf::Color outColor = sf::Color::Black);
};