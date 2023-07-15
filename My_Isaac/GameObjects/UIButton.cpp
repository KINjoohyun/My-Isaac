#include "stdafx.h"
#include "UIButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

UIButton::UIButton(const std::string& fontId, const std::string& name)
	:TextGameObject(fontId, name)
{

}
UIButton::~UIButton()
{

}

void UIButton::Init()
{
	TextGameObject::Init();
	SetOrigin(origin);
}
void UIButton::Reset()
{
	TextGameObject::Reset();

	text.setFillColor({ 255, 255, 255, 255 });
	isHover = false;
}
void UIButton::Update(float dt)
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
void UIButton::Release()
{
	Release();
}

void UIButton::SetText(const std::string& text, int size, sf::Color textColor, float outlineSize, sf::Color outColor)
{
	this->text.setString(text);
	this->text.setCharacterSize(size);
	this->text.setFillColor(textColor);
	this->text.setOutlineThickness(outlineSize);
	this->text.setOutlineColor(outColor);

	SetOrigin(origin);
}