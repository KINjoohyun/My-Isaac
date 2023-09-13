#include "stdafx.h"
#include "InputMgr.h"
#include "Framework.h"
#include "Utils.h"

InputMgr::InputMgr()
{
	AxisInfo infoH; // horizontal info
	infoH.axis = Axis::Horizontal;
	infoH.positives.push_back((int)sf::Keyboard::Key::D);
	infoH.negatives.push_back((int)sf::Keyboard::Key::A);
	axismap.insert({ infoH.axis, infoH });

	AxisInfo infoV; // vertical info
	infoV.axis = Axis::Vertical;
	infoV.positives.push_back((int)sf::Keyboard::Key::S);
	infoV.negatives.push_back((int)sf::Keyboard::Key::W);
	axismap.insert({ infoV.axis, infoV });
}

void InputMgr::Update(float dt)
{
	downList.clear();
	upList.clear();
	textList.clear();

	for (auto& it : axismap)
	{
		auto& axisInfo = it.second;
		float raw = GetAxisRaw(axisInfo.axis);
		if (raw == 0.0f && axisInfo.value != 0.0f)
		{
			raw = axisInfo.value > 0.0f ? -1.0f : 1.0f;
		}
		float diff = axisInfo.sensi * dt;
		axisInfo.value = Utils::Clamp(axisInfo.value + raw * diff, -1.0f, 1.0f);

		if (abs(axisInfo.value) < diff / 2.0f)
		{
			axisInfo.value = 0.0f;
		}
	}
}
void InputMgr::UpdateEvent(const sf::Event& ev)
{
	int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
	switch (ev.type)
	{
	// Keyboard
	case sf::Event::TextEntered:
		if (ev.text.unicode < 128)
		{
			char ch = (char)ev.text.unicode;
			textList.push_back(ch);
		}
		break;
	case sf::Event::KeyPressed:
		if (std::find(ingList.begin(), ingList.end(), ev.key.code) == ingList.end())
		{
			downList.push_back(ev.key.code);
			ingList.push_back(ev.key.code);
		}
		break;
	case sf::Event::KeyReleased:
		ingList.remove(ev.key.code);
		upList.push_back(ev.key.code);
		break;

	// Mouse
	case sf::Event::MouseButtonPressed:
		if (std::find(ingList.begin(), ingList.end(), code) == ingList.end())
		{
			downList.push_back(code);
			ingList.push_back(code);
		}
		break;
	case sf::Event::MouseButtonReleased:
		ingList.remove(code);
		upList.push_back(code);
		break;
	}

	mousePos = (sf::Vector2f)sf::Mouse::getPosition(FRAMEWORK.GetWindow());
}

bool InputMgr::GetKeyDown(sf::Keyboard::Key key)
{
	return std::find(downList.begin(), downList.end(), key) != downList.end();
}
bool InputMgr::GetKey(sf::Keyboard::Key key)
{
	return std::find(ingList.begin(), ingList.end(), key) != ingList.end();
}
bool InputMgr::GetKeyUp(sf::Keyboard::Key key)
{
	return std::find(upList.begin(), upList.end(), key) != upList.end();
}

sf::Vector2f& InputMgr::GetMousePos()
{
	return mousePos;
}
bool InputMgr::GetMouseButtonDown(sf::Mouse::Button button)
{
	int code = sf::Keyboard::KeyCount + (int)button;

	return std::find(downList.begin(), downList.end(), code) != downList.end();
}
bool InputMgr::GetMouseButton(sf::Mouse::Button button)
{
	int code = sf::Keyboard::KeyCount + (int)button;

	return std::find(ingList.begin(), ingList.end(), code) != ingList.end();
}
bool InputMgr::GetMouseButtonUp(sf::Mouse::Button button)
{
	int code = sf::Keyboard::KeyCount + (int)button;

	return std::find(upList.begin(), upList.end(), code) != upList.end();
}

float InputMgr::GetAxis(Axis axis)
{
	const auto& it = axismap.find(axis);
	if (it == axismap.end())
	{
		return 0.0f;
	}

	return it->second.value;
}
float InputMgr::GetAxisRaw(Axis axis)
{
	const auto& it = axismap.find(axis);
	if (it == axismap.end())
	{
		return 0.0f;
	}

	const AxisInfo& inforef = it->second;
	
	auto rit = ingList.rbegin(); // reverse iterator
	while (rit != ingList.rend())
	{
		int code = *rit;

		if (std::find(inforef.positives.begin(), inforef.positives.end(), code) != inforef.positives.end())
		{
			return 1.0f;
		}
		if (std::find(inforef.negatives.begin(), inforef.negatives.end(), code) != inforef.negatives.end())
		{
			return -1.0f;
		}
		rit++;
	}

	return 0.0f;
}
