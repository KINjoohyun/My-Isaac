#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"
#include "ResourceMgr.h"

void Player::Init()
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Move.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Jump.csv"));

	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("Idle");
	SetOrigin(origin);
	SetPosition({ 0, 0 });
	SetFlipX(false);
}

void Player::Update(float dt)
{
	animation.Update(dt);
	float h = INPUT_MGR.GetAxisRaw(Axis::Horizontal);

	// 플립
	if (h != 0.f)
	{
		bool flip = h < 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	// 점프
	if (isGround && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		velocity.y += JumpForce;
		animation.Play("Jump");
		isGround = false;
	}

	// 이동
	velocity.x = h * speed;
	velocity.y += gravity * dt;
	position += velocity * dt;

	// 바닥 충돌 처리
	if (position.y > 0.f)
	{
		isGround = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}

	SetPosition(position);

	// 에니메이션
	if (animation.GetCurrentClipId() == "Idle")
	{
		if (isGround && h != 0.f)
		{
			animation.Play("Move");
		}
	}
	else if (animation.GetCurrentClipId() == "Move")
	{
		if (isGround && h == 0.f)
		{
			animation.Play("Idle");
		}
	}
	else if (animation.GetCurrentClipId() == "Jump")
	{
		if (isGround)
		{
			animation.Play((h == 0.f) ? "Idle" : "Move");
		}
	}
}

bool Player::GetFlipX() const
{
	return flipX;
}
void Player::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flipX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}