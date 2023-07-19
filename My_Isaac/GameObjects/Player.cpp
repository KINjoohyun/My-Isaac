#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

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

	poolTears.OnCreate = [this](Tear* tear)
	{
		tear->pool = &poolTears;
		tear->SetPlayer(this);
	};
	poolTears.Init();

	poolEffects.OnCreate = [this](TearEffect* effect)
	{
		effect->pool = &poolEffects;
	};
	poolEffects.Init();
}
void Player::Reset()
{
	headAnimation.Play("HeadIdleDown");
	bodyAnimation.Play("BodyIdleDown");
	SetPosition(0.0f, 0.0f);
	SetOrigin(origin);
	head.setScale(2.0f, 2.0f);
	body.setScale(2.0f, 2.0f);

	for (auto tear : poolTears.GetUseList())
	{
		SCENE_MGR.GetCurrentScene()->RemoveGO(tear);
	}
	poolTears.AllReturn();

	for (auto effect : poolEffects.GetUseList())
	{
		SCENE_MGR.GetCurrentScene()->RemoveGO(effect);
	}
	poolEffects.AllReturn();

	life = maxLife;
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

	if (!wall.contains(position))
	{
		SetPosition(Utils::Clamp(position, { wallLeft, wallTop }, { wallRight, wallBottom }));
	}

	// Animation
	if (bodyAnimation.GetCurrentClipId() == "BodyIdleDown")
	{
		if (abs(direction.x) > abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveRight");
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
			return;
		}
		if (abs(direction.x) > abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveRight");
		}
	}
	if (bodyAnimation.GetCurrentClipId() == "BodyMoveRight")
	{
		if (direction.x == 0.0f && direction.y == 0.0f)
		{
			bodyAnimation.Play("BodyIdleRight");
			return;
		}
		if (abs(direction.x) < abs(direction.y))
		{
			bodyAnimation.Play("BodyMoveDown");
		}
		SetFlipX(body, direction.x < 0.0f);
	}

	// test
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Left))
	{
		headAnimation.Play("HeadShootLeft");

		TearShoot({-1.0f, 0.0f});
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Right))
	{
		headAnimation.Play("HeadShootRight");

		TearShoot({ 1.0f, 0.0f });
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Up))
	{
		headAnimation.Play("HeadShootUp");

		TearShoot({ 0.0f, -1.0f });
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Down))
	{
		headAnimation.Play("HeadShootDown");

		TearShoot({ 0.0f, 1.0f });
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
	body.setPosition({position});
	head.setPosition(position);
}
void Player::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	body.setPosition(x, y);
	head.setPosition(x, y);
}

void Player::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(body, Origins::C);
		Utils::SetOrigin(head, Origins::BC);
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

void Player::TearShoot(const sf::Vector2f& direction)
{
	Tear* tear = poolTears.Get();
	tear->sortLayer = 1;
	sf::Vector2f headPos =
	{
		head.getGlobalBounds().left + head.getGlobalBounds().width/3,
		head.getGlobalBounds().top + head.getGlobalBounds().height/2
	};
	tear->Shoot(headPos, direction, 500.0f, damage);

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->AddGO(tear);
	}
}
void Player::TearSplash(const sf::Vector2f& tearPos)
{
	TearEffect* effect = poolEffects.Get();
	effect->sortLayer = 1;
	effect->SetPosition(tearPos);

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->AddGO(effect);
	}
}
void Player::OnHit(int damage)
{
	life = std::max(0, life - damage);

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->RenewLife(life);
	}

	if (life <= 0)
	{
		OnDiePlayer();
	}
}
void Player::OnDiePlayer()
{
	std::cout << "DIE" << std::endl;

	Reset();
}
void Player::SetWall(const sf::FloatRect& wall)
{
	this->wall = wall;

	wallTop = wall.top;
	wallBottom = wall.top + wall.height;
	wallLeft = wall.left;
	wallRight = wall.left + wall.width;

	for (auto it : poolTears.GetPool())
	{
		it->SetWall(wall);
	}
}

int Player::GetMaxLife() const
{
	return maxLife;
}
int Player::GetLife() const
{
	return life;
}