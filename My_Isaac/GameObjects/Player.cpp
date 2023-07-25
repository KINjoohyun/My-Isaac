#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

Player::Player(const std::string name)
	:GameObject(name)
{
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyIdleDown.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyIdleRight.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyMoveDown.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyMoveRight.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyHurt.csv"));
	bodyAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/BodyDead.csv"));
	bodyAnimation.SetTarget(&body);

	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleDown.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleRight.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleUp.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadIdleLeft.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootDown.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootRight.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootUp.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadShootLeft.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadHurt.csv"));
	headAnimation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/HeadDead.csv"));
	headAnimation.SetTarget(&head);
}

void Player::Init()
{
	SetOrigin(Origins::C);

	poolTears.OnCreate = [this](Tear* tear)
	{
		tear->pool = &poolTears;
		tear->SetPlayer(this);
	};
	poolTears.Init();

	poolEffects.OnCreate = [this](EffectObject* effect)
	{
		effect->pool = &poolEffects;
		effect->sortLayer = 3;
	};
	poolEffects.Init();
}
void Player::Reset()
{
	headAnimation.Play("HeadIdleDown");
	bodyAnimation.Play("BodyIdleDown");
	SetPosition(0.0f, 0.0f);
	head.setScale(2.0f, 2.0f);
	headCol.setSize({ head.getGlobalBounds().width, head.getGlobalBounds().height });
	headCol.setFillColor(sf::Color::Transparent);
	headCol.setOutlineThickness(1);
	body.setScale(2.0f, 2.0f);
	bodyCol.setSize({ body.getGlobalBounds().width, body.getGlobalBounds().height });
	bodyCol.setFillColor(sf::Color::Transparent);
	bodyCol.setOutlineThickness(1);
	SetOrigin(origin);

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
	invincibleTimer = invincibleDuration;
	attackTimer = attackDuration;
	isAlive = true;

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->RenewLife(life);
	}

	for (auto it : poolTears.GetPool())
	{
		it->SetWall(wall);
		it->OnDebug = [scene, it]()
		{
			if (scene->isDebug)
			{
				it->col.setOutlineColor(sf::Color::White);
			}
			else
			{
				it->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}
}
void Player::Update(float dt)
{
	if (OnDebug != nullptr) OnDebug();

	headAnimation.Update(dt);
	bodyAnimation.Update(dt);
	if (!isAlive) return;

	if (invincibleTimer < invincibleDuration)
	{
		invincibleTimer += dt;
		head.setColor((head.getColor() == sf::Color::White) ? sf::Color::Yellow : sf::Color::White);
		body.setColor((body.getColor() == sf::Color::White) ? sf::Color::Yellow : sf::Color::White);
	}
	else if (head.getColor() != sf::Color::White)
	{
		head.setColor(sf::Color::White);
		body.setColor(sf::Color::White);
	}

	if (attackTimer < attackDuration)
	{
		attackTimer += dt;
	}

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

	if (INPUT_MGR.GetKey(sf::Keyboard::Left))
	{
		if (attackTimer < attackDuration) return;

		headAnimation.Play("HeadShootLeft");

		TearShoot({-1.0f, 0.0f});
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Right))
	{
		if (attackTimer < attackDuration) return;

		headAnimation.Play("HeadShootRight");

		TearShoot({ 1.0f, 0.0f });
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Up))
	{
		if (attackTimer < attackDuration) return;

		headAnimation.Play("HeadShootUp");

		TearShoot({ 0.0f, -1.0f });
	}
	if (INPUT_MGR.GetKey(sf::Keyboard::Down))
	{
		if (attackTimer < attackDuration) return;

		headAnimation.Play("HeadShootDown");

		TearShoot({ 0.0f, 1.0f });
	}
}
void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	window.draw(head);

	window.draw(bodyCol);
	window.draw(headCol);
}

void Player::SetPosition(const sf::Vector2f& position)
{
	GameObject::SetPosition(position);
	body.setPosition(position);
	bodyCol.setPosition(position);
	head.setPosition(position);
	headCol.setPosition(position);
}
void Player::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	body.setPosition(x, y);
	bodyCol.setPosition(x, y);
	head.setPosition(x, y);
	headCol.setPosition(x, y);
}

void Player::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);

	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(body, Origins::C);
		Utils::SetOrigin(bodyCol, Origins::C);
		Utils::SetOrigin(head, Origins::BC);
		Utils::SetOrigin(headCol, Origins::BC);
	}
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
	attackTimer = 0.0f;

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
		tear->SetHitlist(scene->GetPoopList());
		scene->AddGO(tear);
	}
}
void Player::Splash(const sf::Vector2f& tearPos, const std::string& anim)
{
	EffectObject* effect = poolEffects.Get();
	effect->sortLayer = 1;
	effect->SetPosition(tearPos);
	effect->SetAnimation(anim);

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->AddGO(effect);
	}
}
void Player::OnHit(int damage)
{
	if (invincibleTimer < invincibleDuration) return;

	life = std::max(0, life - damage);
	invincibleTimer = 0.0f;
	headAnimation.Play("HeadHurt");
	headAnimation.PlayQueue("HeadIdleDown");
	bodyAnimation.Play("BodyHurt");
	bodyAnimation.PlayQueue("BodyIdleDown");

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->RenewLife(life);
	}

	if (life <= 0)
	{
		isAlive = false;
		body.setColor(sf::Color::White);
		head.setColor(sf::Color::White);
		bodyAnimation.Play("BodyDead");
		headAnimation.Play("HeadDead");

		scene->OnDiePlayer();
	}
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

void Player::IncreaseLife(int life)
{
	this->life = std::min(maxLife, this->life + life);
	invincibleTimer = 0.0f;

	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (scene != nullptr)
	{
		scene->RenewLife(this->life);
	}
}
void Player::ChangeSpeed(float value)
{
	this->speed = std::max(0.0f, this->speed + value);
}