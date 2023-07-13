#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "ResourceMgr.h"

Player::Player(const std::string name)
	:GameObject(name)
{

}

void Player::Init()
{
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyIdleDown.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyIdleRight.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyMoveDown.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyMoveRight.csv"));
	bodyAnimation.SetTarget(&body);

	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleDown.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleRight.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleUp.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleLeft.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootDown.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootRight.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootUp.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootLeft.csv"));
	headAnimation.SetTarget(&head);

	SetOrigin(Origins::C);
}
void Player::Reset()
{
	headAnimation.Play("HeadIdleDown");
	bodyAnimation.Play("BodyIdleDown");
	SetPosition(0.0f, 0.0f);
	SetOrigin(origin);
}
void Player::Update(float dt)
{
	headAnimation.Update(dt);
	bodyAnimation.Update(dt);

	direction.x = INPUT_MGR.GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxisRaw(Axis::Vertical);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	position += direction * speed * dt;
	SetPosition(position);

	// Animation
	if (bodyAnimation.GetCurrentClipId() == "BodyIdleDown")
	{
		if (abs(direction.x) > abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveRight");
			SetFlipX(body, direction.x < 0.0f);
		}
		if (abs(direction.x) < abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveDown");
		}
	}
	if (bodyAnimation.GetCurrentClipId() == "BodyIdleRight")
	{
		if (abs(direction.x) > abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveRight");
			SetFlipX(body, direction.x < 0.0f);
		}
		if (abs(direction.x) < abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveDown");
		}
	}
	if (bodyAnimation.GetCurrentClipId() == "BodyMoveDown")
	{
		if (direction.x == 0.0f && direction.y == 0.0f)
		{
			bodyAnimation.Play("BodyIdleDown");
		}
		if (abs(direction.x) > abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveRight");
			SetFlipX(body, direction.x < 0.0f);
		}
	}
	if (bodyAnimation.GetCurrentClipId() == "BodyMoveRight")
	{
		if (direction.x == 0.0f && direction.y == 0.0f)
		{
			bodyAnimation.Play("BodyIdleRight");
		}
		if (abs(direction.x) < abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveDown");
		}
	}

	// test
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Left))
	{
		headAnimation.Play("HeadShootLeft");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Right))
	{
		headAnimation.Play("HeadShootRight");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Up))
	{
		headAnimation.Play("HeadShootUp");
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Down))
	{
		headAnimation.Play("HeadShootDown");
	}
	
}
void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	window.draw(head);
}

void Player::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	body.setPosition({position.x, position.y + 13.0f});
	head.setPosition(position);
}
void Player::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	body.setPosition(x, y + 13.0f);
	head.setPosition(x, y);
}

void Player::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(body, origin);
		Utils::SetOrigin(head, origin);
	}
}
void Player::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	body.setOrigin(x, y);
	head.setOrigin(x, y);
}

bool Player::GetFlipX() const
{
	return flipX;
}
void Player::SetFlipX(sf::Sprite& sprite, bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flipX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}