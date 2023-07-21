#include "stdafx.h"
#include "SceneGame.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "SpriteGameObject.h"
#include "rapidcsv.h"
#include "Tile.h"
#include "RectGameObject.h"
#include "Player.h"
#include "RoomObject.h"
#include "Monster.h"
#include "Door.h"
#include "Blood.h"
#include "Boss.h"

SceneGame::SceneGame() :Scene(SceneId::Game)
{
	resourceListPath = "scripts/SceneGameResourceList.csv";
}
SceneGame::~SceneGame()
{
	Release();
}

void SceneGame::Init()
{
	Release();

	worldView.setSize(windowSize);
	worldView.setCenter(0, -100);

	uiView.setSize(windowSize);
	uiView.setCenter({ windowSize.x * 0.5f, windowSize.y * 0.5f });

	SpriteGameObject* ui_bg = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_bg.png"));
	ui_bg->SetOrigin(Origins::TL);
	ui_bg->sortLayer = 100;

	player = (Player*)AddGO(new Player());
	player->sortLayer = 2;

	// 하드 코딩으로 랜덤한 Room 호출
	std::string randomPath1 = "room/Spawn.csv";
	CallRoom(randomPath1, { 0.0f, 0.0f });

	std::string randomPath2 = "room/Room" + std::to_string(Utils::RandomRange(1, 9)) + ".csv";
	CallRoom(randomPath2, { 1100.0f, 0.0f });

	std::string randomPath3 = "room/Room" + std::to_string(Utils::RandomRange(1, 9)) + ".csv";
	CallRoom(randomPath3, { -1100.0f, 0.0f });

	std::string randomPath4 = "room/Boss1.csv";
	CallRoom(randomPath4, { 0.0f, -1100.0f });

	// 최대체력 그대로 생성중
	for (int i = 0; i < player->GetMaxLife(); i++)
	{
		SpriteGameObject* ui_heart = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_heart0.png"));
		ui_heart->SetOrigin(Origins::TC);
		ui_heart->SetPosition({ windowSize.x - 200.0f + (60.0f * i), 50.0f});
		ui_heart->sortLayer = 100;
	}
	RenewLife(player->GetMaxLife());

	// test code
	Door* door1 = (Door*)AddGO(new Door("graphics/door_open.png", {400.0f, 0.0f}, Door::Look::Right));
	Door* door2 = (Door*)AddGO(new Door("graphics/door_open.png", {-400.0f, 0.0f}, Door::Look::Left));
	door1->SetPlayer(player);
	door2->SetPlayer(player);
	door1->Open();
	door2->Open();
	door1->SetDestination({ 1100.0f, 0.0f });
	door2->SetDestination({ -1100.0f, 0.0f });

	Door* door3 = (Door*)AddGO(new Door("graphics/door_open.png", { -700.0f, 0.0f }, Door::Look::Right));
	door3->SetPlayer(player);
	door3->Open();
	door3->SetDestination({ 0.0f, 0.0f });

	Door* door4 = (Door*)AddGO(new Door("graphics/door_open.png", { 700.0f, 0.0f }, Door::Look::Left));
	door4->SetPlayer(player);
	door4->Open();
	door4->SetDestination({ 0.0f, 0.0f });

	Door* door5 = (Door*)AddGO(new Door("graphics/door_open.png", { 0.0f, -250.0f }, Door::Look::Up));
	Door* door6 = (Door*)AddGO(new Door("graphics/door_open.png", { 0.0f, -850.0f }, Door::Look::Down));
	door5->SetPlayer(player);
	door6->SetPlayer(player);
	door5->Open();
	door6->Open();
	door5->SetDestination({ 0.0f, -1100.0f });
	door6->SetDestination({ 0.0f, -0.0f });

	// test code
	RectGameObject* wall = (RectGameObject*)FindGO(randomPath1);
	player->SetWall(wall->rect.getGlobalBounds());

	poolBloods.OnCreate = [this, wall](Blood* blood)
	{
		blood->pool = &poolBloods;
		blood->SetPlayer(player);
		blood->sortLayer = 1;
		blood->SetWall(wall->rect.getGlobalBounds());
	};
	poolBloods.Init();

	for (auto go : gameObjects)
	{
		go->Init();
	}
}
void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}

	// test
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::T))
	{
		Blood* blood = poolBloods.Get();
		blood->Shoot({ 0.0f, 0.0f }, { 1.0f, 0.0f }, 300.0f, 1);
		AddGO(blood);
	}
}
void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
void SceneGame::Release()
{
	poolBloods.Release();

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	Scene::Enter();

	ClearPool(poolBloods);
}
void SceneGame::Exit()
{
	ClearPool(poolBloods);
	player->Reset();

	Scene::Exit();
}

void SceneGame::CallRoom(const std::string& roomPath, const sf::Vector2f& position)
{
	rapidcsv::Document doc(roomPath, rapidcsv::LabelParams(-1, -1));
	std::string bg = doc.GetCell<std::string>(0, 1);
	
	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject(bg, "bg"));
	background->SetOrigin(Origins::C);
	background->SetPosition(position);

	int sizex = doc.GetCell<int>(1, 1);
	int sizey = doc.GetCell<int>(2, 1);

	RectGameObject* wall = (RectGameObject*)AddGO(new RectGameObject(roomPath));
	wall->rect.setSize({ (float)sizex, (float)sizey });
	wall->rect.setOutlineColor(sf::Color::Red);
	wall->rect.setOutlineThickness(1);
	wall->rect.setFillColor(sf::Color::Transparent);
	wall->SetOrigin(Origins::C);
	wall->SetPosition(position);

	for (int i = 4; i < doc.GetRowCount(); i++)
	{
		auto rows = doc.GetRow<std::string>(i);
		auto obj = LoadObj((ObjType)std::stoi(rows[0]), rows[1], wall->rect.getGlobalBounds());
		obj->SetOrigin(Origins::C);
		obj->SetPosition(position.x + std::stof(rows[2]), position.y + std::stof(rows[3]));
		obj->sortLayer = 1;
		obj->sortOrder = std::stoi(rows[4]);
	}
}

void SceneGame::RenewLife(int life)
{
	for (auto it : lifebar)
	{
		RemoveGO(it);
	}
	lifebar.clear();

	// 현재 1하트 단위
	for (int i = 0; i < life; i++)
	{
		SpriteGameObject* ui_heart = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_heart2.png"));
		ui_heart->SetOrigin(Origins::TC);
		ui_heart->SetPosition({ windowSize.x - 200.0f + (60.0f * i), 50.0f });
		ui_heart->sortLayer = 101;
		ui_heart->Init();
		ui_heart->Reset();
		lifebar.push_back(ui_heart);
	}
}
void SceneGame::ViewSet(const sf::Vector2f& position)
{
	worldView.setCenter({position.x, position.y - 100.0f});
}
SpriteGameObject* SceneGame::LoadObj(ObjType objtype, const std::string& textureId, const sf::FloatRect& wall)
{
	switch (objtype)
	{
	case ObjType::None:
	{
		RoomObject* none = (RoomObject*)AddGO(new RoomObject(textureId));
		return (SpriteGameObject*)none;
	}
	break;
	case ObjType::Rock:
	{
		RoomObject* rock = (RoomObject*)AddGO(new RoomObject(textureId));
		rock->SetPlayer(player);
		rock->OnBump = [this, rock]()
		{
			player->SetPosition(player->GetPosition() - Utils::Normalize(rock->GetPosition() - player->GetPosition()));
		};
		rock->SetWall(wall);
		hitablelist.push_back(rock);
		return (SpriteGameObject*)rock;
	}
	break;
	case ObjType::Poop:
	{
		RoomObject* poop = (RoomObject*)AddGO(new RoomObject(textureId));
		poop->SetMaxHp(4);
		poop->SetPlayer(player);
		poop->OnHit = [poop](int damage)
		{
			poop->OnDamage(damage);
		};
		poop->OnBump = [this, poop]()
		{
			player->SetPosition(player->GetPosition() - Utils::Normalize(poop->GetPosition() - player->GetPosition()));
		};
		poop->SetWall(wall);
		hitablelist.push_back(poop);
		return (SpriteGameObject*)poop;
	}
	break;
	case ObjType::Spike:
	{
		RoomObject* spike = (RoomObject*)AddGO(new RoomObject(textureId));
		spike->SetPlayer(player);
		spike->OnBump = [this, spike]()
		{
			player->OnHit(1);
		};
		spike->SetWall(wall);
		return (SpriteGameObject*)spike;
	}
	break;
	case ObjType::AttackFly:
	{
		Monster* attackfly = (Monster*)AddGO(new Monster(objtype));
		attackfly->SetPlayer(player);
		attackfly->SetMonster(1, 150.0f, 3, 500.0f);
		attackfly->OnBump = [this, attackfly]()
		{
			player->OnHit(attackfly->GetDamage());
		};
		attackfly->SetWall(wall);
		hitablelist.push_back(attackfly);
		return (SpriteGameObject*)attackfly;
	}
	break;
	case ObjType::Pooter:
	{
		Monster* pooter = (Monster*)AddGO(new Monster(objtype));
		pooter->SetPlayer(player);
		pooter->SetMonster(1, 100.0f, 4, 400.0f);
		pooter->OnBump = [this, pooter]()
		{
			player->OnHit(pooter->GetDamage());
		};
		pooter->BloodShoot = [this, pooter, wall]()
		{
			Blood* blood = poolBloods.Get();
			blood->SetWall(wall);
			blood->Shoot(pooter->GetPosition(), pooter->GetDirection(), 300.0f, 1);
			AddGO(blood);
		};
		pooter->SetWall(wall);
		hitablelist.push_back(pooter);
		return (SpriteGameObject*)pooter;
	}
	break;
	case ObjType::Sucker:
	{
		Monster* sucker = (Monster*)AddGO(new Monster(objtype));
		sucker->SetPlayer(player);
		sucker->SetMonster(1, 100.0f, 5, 400.0f);
		sucker->OnBump = [this, sucker]()
		{
			player->OnHit(sucker->GetDamage());
		};
		sucker->OnDie = [this, sucker, wall]()
		{
			Blood* blood1 = poolBloods.Get();
			blood1->SetWall(wall);
			blood1->Shoot(sucker->GetPosition(), { -1.0f, 0.0f }, 300.0f, 1);
			AddGO(blood1);

			Blood* blood2 = poolBloods.Get();
			blood2->SetWall(wall);
			blood2->Shoot(sucker->GetPosition(), { 0.0f, -1.0f }, 300.0f, 1);
			AddGO(blood2);

			Blood* blood3 = poolBloods.Get();
			blood3->SetWall(wall);
			blood3->Shoot(sucker->GetPosition(), { 1.0f, 0.0f }, 300.0f, 1);
			AddGO(blood3);

			Blood* blood4 = poolBloods.Get();
			blood4->SetWall(wall);
			blood4->Shoot(sucker->GetPosition(), { 0.0f, 1.0f }, 300.0f, 1);
			AddGO(blood4);

			RemoveRGO(sucker);
		};
		sucker->SetWall(wall);
		hitablelist.push_back(sucker);
		return (SpriteGameObject*)sucker;
	}
	break;
	default:
	{
		Tile* tile = (Tile*)AddGO(new Tile(objtype, textureId));
		return (SpriteGameObject*)tile;
	}
	break;
	case ObjType::DukeOfFlies:
	{
		Boss* duke = (Boss*)AddGO(new Boss(objtype, textureId));
		duke->SetPlayer(player);
		duke->SetMonster(1, 100.0f, 50, 800.0f, true);
		duke->OnBump = [this, duke]()
		{
			player->SetPosition(player->GetPosition() - Utils::Normalize(duke->GetPosition() - player->GetPosition()));
			player->OnHit(duke->GetDamage());
		};
		duke->Pattern1 = [this, duke, wall]()
		{
			if (Utils::Distance(duke->GetPosition(), player->GetPosition()) > 200.0f)
			{
				return false;
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					Monster* attackfly = (Monster*)AddGO(new Monster(ObjType::AttackFly));
					attackfly->SetPlayer(player);
					attackfly->SetMonster(1, 150.0f, 3, 500.0f);
					attackfly->OnBump = [this, attackfly]()
					{
						player->OnHit(1);
					};
					attackfly->SetWall(wall);
					attackfly->SetOrigin(Origins::C);
					attackfly->SetPosition(duke->GetPosition().x - 100.0f + (200.0f * i), duke->GetPosition().y);
					attackfly->sortLayer = 1;
					attackfly->sortOrder = 1;
					hitablelist.push_back(attackfly);
					attackfly->Init();
					attackfly->Reset();
				}
				
				return true;
			}
		};
		duke->SetCooltime(3.0f, 5.0f);
		duke->SetWall(wall);
		hitablelist.push_back(duke);
		return (SpriteGameObject*)duke;
	}
	}
}
const std::list<RoomObject*>* SceneGame::GetPoopList() const
{
	return &hitablelist;
}
void SceneGame::RemoveRGO(RoomObject* roomGO)
{
	hitablelist.remove(roomGO);
	RemoveGO(roomGO);
}