#include "stdafx.h"
#include "UITextButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

UITextButton::UITextButton(const std::string& fontId, const std::string& name)
	:TextGameObject(fontId, name)
{

}
UITextButton::~UITextButton()
{

}

void UITextButton::Init()
{
	TextGameObject::Init();
	SetOrigin(origin);
}
void UITextButton::Reset()
{
	TextGameObject::Reset();

	text.setFillColor({ 255, 255, 255, 255 });
	isHover = false;
}
void UITextButton::Update(float dt)
{
	TextGameObject::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousPos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

	bool prevHover = isHover;
	isHover = text.getGlobalBounds().contains(uiMousPos);

	if (!prevHover && isHover)
	{
		if (OnEnter != nullptr)
		{
			text.setFillColor({ 255, 255, 255, 100 });
			OnEnter();
		}
	}

	if (prevHover && !isHover)
	{
		if (OnExit != nullptr)
		{
			text.setFillColor({ 255, 255, 255, 255 });
			OnExit();
		}
	}

	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		if (OnClick != nullptr)
		{
			Reset();
			OnClick();
		}
	}

}
void UITextButton::Release()
{
	Release();
}

void UITextButton::SetText(const std::string& text, int size, sf::Color textColor, float outlineSize, sf::Color outColor)
{
	this->text.setString(text);
	this->text.setCharacterSize(size);
	this->text.setFillColor(textColor);
	this->text.setOutlineThickness(outlineSize);
	this->text.setOutlineColor(outColor);

	SetOrigin(origin);
}