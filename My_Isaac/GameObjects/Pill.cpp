#include "stdafx.h"
#include "Pill.h"
#include "Player.h"
#include "SceneMgr.h"
#include "SceneGame.h"

void Pill::Init()
{
	SpriteGameObject::Init();

	int ran = Utils::RandomRange(0, 12);
	switch (ran)
	{
	case 0:
		sprite.setTextureRect({ 7, 5, 17, 17 });
		break;
	case 1:
		sprite.setTextureRect({ 42, 6, 13, 19 });
		break;
	case 2:
		sprite.setTextureRect({ 72, 6, 17, 17 });
		break;
	case 3:
		sprite.setTextureRect({ 103, 9, 19, 13 });
		break;
	case 4:
		sprite.setTextureRect({ 10, 38, 13, 19 });
		break;
	case 5:
		sprite.setTextureRect({ 39, 37, 17, 17 });
		break;
	case 6:
		sprite.setTextureRect({ 73, 38, 17, 17 });
		break;
	case 7:
		sprite.setTextureRect({ 106, 38, 13, 19 });
		break;
	case 8:
		sprite.setTextureRect({ 10, 70, 13, 19 });
		break;
	case 9:
		sprite.setTextureRect({ 39, 70, 17, 17 });
		break;
	case 10:
		sprite.setTextureRect({ 73, 69, 17, 17 });
		break;
	case 11:
		sprite.setTextureRect({ 104, 70, 17, 17 });
		break;
	case 12:
		sprite.setTextureRect({ 103, 105, 19, 13 });
		break;
	}
}
void Pill::Reset()
{
	SpriteGameObject::Reset();

	SetOrigin(Origins::C);
	sprite.setScale(2.0f, 2.0f);
}
void Pill::Update(float dt)
{
	SpriteGameObject::Update(dt);

	if (sprite.getGlobalBounds().intersects(player->body.getGlobalBounds()))
	{
		SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();

		switch (type)
		{
		case PillType::None:
			break;
		case PillType::FullHealth:
			player->IncreaseLife(999);
			scene->PrintGuide("FULL HEALTH");
			break;
		case PillType::HealthDown:
			player->OnHit(1);
			scene->PrintGuide("HEALTH DOWN");
			break;
		case PillType::HealthUp:
			player->IncreaseLife(1);
			scene->PrintGuide("HEALTH UP");
			break;
		case PillType::SpeedDown:
			player->ChangeSpeed(-70.0f);
			scene->PrintGuide("SPEED DOWN");
			break;
		case PillType::SpeedUp:
			player->ChangeSpeed(+70.0f);
			scene->PrintGuide("SPEED UP");
			break;
		default:
			std::cout << "WARNING: Not Exist PillType" << std::endl;
			break;
		}

		scene->SoundPlaying("sounds/pillpickup.ogg");
		scene->RemoveGO(this);
	}
}

void Pill::SetPlayer(Player* player)
{
	this->player = player;
}
void Pill::SetType(PillType type)
{
	this->type = type;
}