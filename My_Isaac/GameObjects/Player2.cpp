#include "stdafx.h"
#include "Player2.h"
#include "InputMgr.h"
#include "ResourceMgr.h"

Player2::Player2(const std::string textureId, const std::string name)
{

}

void Player2::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Move0.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Move1.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Move2.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Idle0.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Idle1.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Idle2.csv"));

	animation.SetTarget(&sprite);

	SetOrigin(Origins::C);
}

void Player2::Reset()
{
	//SpriteGameObject::Reset();

	animation.Play("Idle" + std::to_string((int)Look::Down));
	SetPosition(0.0f, 0.0f);
	SetOrigin(origin);
	look = Look::Down;
}

void Player2::Update(float dt)
{
	SpriteGameObject::Update(dt);
	animation.Update(dt);

	direction.x = INPUT_MGR.GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxisRaw(Axis::Vertical);

	SetPosition({ position.x + direction.x * speed * dt, position.y + direction.y * speed * dt });

	// Animation
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Left))
	{
		SetFlipX(false);
		animation.Play("Move" + std::to_string((int)Look::Left));
		look = Look::Left;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Right))
	{
		SetFlipX(true);
		animation.Play("Move" + std::to_string((int)Look::Left));
		look = Look::Left;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Up))
	{
		animation.Play("Move" + std::to_string((int)Look::Up));
		look = Look::Up;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Down))
	{
		animation.Play("Move" + std::to_string((int)Look::Down));
		look = Look::Down;
	}

	for (int i = 0; i < 3; i++)
	{
		if (direction.x == 0.0f && direction.y == 0.0f && animation.GetCurrentClipId() == "Move" + std::to_string(i))
		{
			animation.Play("Idle" + std::to_string((int)look));
		}
	}

	if (animation.GetCurrentClipId() == "Move" + std::to_string((int)Look::Left)) // MoveLeft
	{
		if (abs(direction.y) > abs(direction.x) && direction.y > 0.0f)
		{
			animation.Play("Move" + std::to_string((int)Look::Down)); // MoveDown
		}
		else if (abs(direction.y) > abs(direction.x) && direction.y < 0.0f)
		{
			animation.Play("Move" + std::to_string((int)Look::Up)); // MoveUp
		}
	}
	if (animation.GetCurrentClipId() == "Move" + std::to_string((int)Look::Up)) // MoveUp
	{
		if (abs(direction.x) > abs(direction.y))
		{
			animation.Play("Move" + std::to_string((int)Look::Left)); // MoveLeft
		}
		else if (direction.y > 0.0f)
		{
			animation.Play("Move" + std::to_string((int)Look::Down)); // MoveDown
		}
	}
	if (animation.GetCurrentClipId() == "Move" + std::to_string((int)Look::Down)) // MoveDown
	{
		if (abs(direction.x) > abs(direction.y))
		{
			animation.Play("Move" + std::to_string((int)Look::Left)); // MoveLeft
		}
		else if (direction.y < 0.0f)
		{
			animation.Play("Move" + std::to_string((int)Look::Up)); // MoveUp
		}
	}
}

bool Player2::GetFlipX() const
{
	return flipX;
}
void Player2::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flipX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}