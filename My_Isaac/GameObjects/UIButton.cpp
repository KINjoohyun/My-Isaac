#include "stdafx.h"
#include "UIButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"

UIButton::UIButton(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{

}
UIButton::~UIButton()
{

}

void UIButton::Init()
{
	SpriteGameObject::Init();

}
void UIButton::Reset()
{
	SpriteGameObject::Reset();

	isHover = false;
}
void UIButton::Update(float dt)
{
	SpriteGameObject::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousPos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(uiMousPos);

	if (!prevHover && isHover)
	{
		if (OnEnter != nullptr)
		{
			OnEnter();
		}
	}

	if (prevHover && !isHover)
	{
		if (OnExit != nullptr)
		{
			OnExit();
		}
	}

	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
	{
		if (OnClick != nullptr)
		{
			OnClick();
		}
	}

}
void UIButton::Draw(sf::RenderWindow& window)
{
	SpriteGameObject::Draw(window);
	window.draw(text);
}
void UIButton::Release()
{
	Release();
}