#include "stdafx.h"
#include "UIImageButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

UIImageButton::UIImageButton(const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{

}
UIImageButton::~UIImageButton()
{

}

void UIImageButton::Init()
{
	SpriteGameObject::Init();
}
void UIImageButton::Reset()
{
	SpriteGameObject::Reset();

	isHover = false;
}
void UIImageButton::Update(float dt)
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
			Reset();
			OnClick();
		}
	}
}
void UIImageButton::Draw(sf::RenderWindow& window)
{
	SpriteGameObject::Draw(window);
}