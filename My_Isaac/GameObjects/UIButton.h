#pragma once
#include "TextGameObject.h"

class UIButton : public TextGameObject
{
protected:
	bool isHover = false;
public:
	UIButton(const std::string& fontId = "", const std::string& name = "");
	virtual ~UIButton() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override;

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	void SetText(const std::string& text = "", int size = 0, sf::Color textColor = sf::Color::Black, float outlineSize = 0.0f, sf::Color outColor = sf::Color::Black);
};