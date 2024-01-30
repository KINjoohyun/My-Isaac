#include "stdafx.h"
#include "TextBox.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

TextBox::TextBox(const std::string& fontId, const std::string& str, const std::string& n, Origins textsort)
	:GameObject(n), textsort(textsort), fontId(fontId)
{
	text.setString(str);
	box.setFillColor({128, 128, 128});
	box.setOutlineColor(sf::Color::White);
	box.setOutlineThickness(1);
	text.setFillColor(sf::Color::Black);
}

TextBox::~TextBox()
{
}

void TextBox::SetPosition(const sf::Vector2f& p)
{
	GameObject::SetPosition(p);
	box.setPosition(p);
	text.setPosition(p);
}

void TextBox::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	box.setPosition(x, y);
	text.setPosition(x, y);
}

void TextBox::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	Utils::SetOrigin(box, origin);
	Utils::SetOrigin(text, textsort);
}

void TextBox::Init()
{
}

void TextBox::Release()
{
}

void TextBox::Reset()
{
	sf::Font* font = RESOURCE_MGR.GetFont(fontId);
	if (font != nullptr)
	{
		text.setFont(*font);
	}

	box.setFillColor({ 128, 128, 128 });
	text.setFillColor(sf::Color::Black);

	SetOrigin(origin);
	isTyping = false;
}

void TextBox::Update(float dt)
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousePos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

	bool isHover = box.getGlobalBounds().contains(uiMousePos);

	if (isHover && INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		isTyping = true;
		box.setFillColor(sf::Color::White);
		text.setFillColor(sf::Color::Black);
	}
	if (isTyping)
	{
		Typing();
	}
}

void TextBox::Draw(sf::RenderWindow& window)
{
	window.draw(box);
	window.draw(text);
}

void TextBox::Typing()
{
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		Reset();

		return;
	}
	else if (!INPUT_MGR.textList.empty())
	{
		if (INPUT_MGR.textList.front() == '\b')
		{
			std::string s = text.getString();
			if (s.empty()) return;
			s.pop_back();
			text.setString(s);

			SetOrigin(origin);
		}
		else
		{
			std::string s = text.getString();
			text.setString(s + INPUT_MGR.textList.front());

			SetOrigin(origin);
		}
	}
}
