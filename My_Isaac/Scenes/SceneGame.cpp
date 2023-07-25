#include "stdafx.h"
#include "SceneGame.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "SpriteGameObject.h"
#include "TextGameObject.h"
#include "rapidcsv.h"
#include "Tile.h"
#include "RectGameObject.h"
#include "Player.h"
#include "RoomObject.h"
#include "Monster.h"
#include "Door.h"
#include "Blood.h"
#include "Boss.h"
#include "MiniMap.h"
#include "Pill.h"

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

	fpstext = (TextGameObject*)AddGO(new TextGameObject("fonts/DNFBitBitOTF.otf"));
	fpstext->SetOrigin(Origins::TL);
	fpstext->text.setCharacterSize(10);
	fpstext->text.setFillColor(sf::Color::Green);
	fpstext->sortLayer = 105;
	fpstext->action = [&]() // & capture
	{
		fpstext->SetActive(isDebug);
	};
	fpstext->SetActive(false);

	SpriteGameObject* ui_bg = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_bg.png"));
	ui_bg->SetOrigin(Origins::TL);
	ui_bg->sortLayer = 100;

	player = (Player*)AddGO(new Player());
	player->OnDebug = [this]()
	{
		if (isDebug)
		{
			player->bodyCol.setOutlineColor(sf::Color::White);
			player->headCol.setOutlineColor(sf::Color::White);
		}
		else
		{
			player->bodyCol.setOutlineColor(sf::Color::Transparent);
			player->headCol.setOutlineColor(sf::Color::Transparent);
		}
	};
	player->sortLayer = 2;

	RandomRooms();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SetDoor(i, j);
		}
	}

	minimap = (MiniMap*)AddGO(new MiniMap("graphics/ui/minimap1.png"));
	minimap->SetPosition({ 110.0f, 100.0f });
	minimap->sortLayer = 101;
	
	// 최대체력 그대로 생성중
	for (int i = 0; i < player->GetMaxLife(); i++)
	{
		SpriteGameObject* ui_heart = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_heart0.png"));
		ui_heart->SetOrigin(Origins::TC);
		ui_heart->SetPosition({ windowSize.x - 200.0f + (60.0f * i), 50.0f });
		ui_heart->sortLayer = 100;
	}
	RenewLife(player->GetMaxLife());

	RectGameObject* wall = (RectGameObject*)FindGO("room/Spawn.csv");
	poolBloods.OnCreate = [this, wall](Blood* blood)
	{
		blood->pool = &poolBloods;
		blood->SetPlayer(player);
		blood->sortLayer = 1;
		blood->OnDebug = [this, blood]()
		{
			if (isDebug)
			{
				blood->col.setOutlineColor(sf::Color::Green);
			}
			else
			{
				blood->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	};
	poolBloods.Init();

	pauseObject = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_pause.png"));
	pauseObject->SetOrigin(Origins::C);
	pauseObject->sortLayer = 100;
	pauseObject->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f);
	pauseObject->SetActive(false);

	SpriteGameObject* gameover = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/gameover.png", "gameover"));
	gameover->SetOrigin(Origins::C);
	gameover->sortLayer = 100;
	gameover->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f);
	gameover->SetActive(false);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}
void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	fps++;
	fpstimer += dt;
	if (fpstimer >= 1.0f)
	{
		fpstext->text.setString("FPS : " + std::to_string(fps));
		fpstimer = 0.0f;
		fps = 0.0f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape) && isAlive)
	{
		pauseObject->SetActive(!pauseObject->GetActive());
	}

	// Debug Mode
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F5))
	{
		isDebug = !isDebug;
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

	isAlive = true;
	RectGameObject* wall = (RectGameObject*)FindGO("room/Spawn.csv");
	player->SetWall(wall->rect.getGlobalBounds());
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{ 
			if (stage1[i][j].tag == 'S')
			{
				player->SetPosition(stage1[i][j].pos);
				break;
			}
		}
	}
	ViewSet(player->GetPosition());
}
void SceneGame::Exit()
{
	ClearPool(poolBloods);
	player->Reset();

	Scene::Exit();
}

void SceneGame::CallRoom(const std::string& roomPath, const sf::Vector2f& position, int r, int c)
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
	wall->rect.setFillColor(sf::Color::Transparent);
	wall->rect.setOutlineThickness(1);
	wall->SetOrigin(Origins::C);
	wall->SetPosition(position);
	wall->OnDebug = [this, wall]()
	{
		if (isDebug)
		{
			wall->rect.setOutlineColor(sf::Color::Blue);
		}
		else
		{
			wall->rect.setOutlineColor(sf::Color::Transparent);
		}
	};

	for (int i = 4; i < doc.GetRowCount(); i++)
	{
		auto rows = doc.GetRow<std::string>(i);
		auto obj = LoadObj((ObjType)std::stoi(rows[0]), rows[1], wall->rect.getGlobalBounds(), r, c);
		obj->SetOrigin(Origins::C);
		obj->SetPosition(position.x + std::stof(rows[2]), position.y + std::stof(rows[3]));
		obj->sortLayer = 1;
		obj->sortOrder = std::stoi(rows[4]);
		obj->OnDebug = [this, obj]()
		{
			if (isDebug)
			{
				obj->col.setOutlineColor(sf::Color::Red);
			}
			else
			{
				obj->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}

	(roomPath == "room/Spawn.csv") ?
		(stage1[r][c].tag = 'S', stage1[r][c].isHear = true) :
		stage1[r][c].tag = 'N';
	stage1[r][c].wall = wall->rect.getGlobalBounds();
	stage1[r][c].pos = position;
}
void SceneGame::SetDoor(int r, int c)
{
	if (stage1[r][c].tag == NULL) return;

	if (stage1[r][c - 1].tag != NULL && c > 0)
	{
		std::string doorPath = (stage1[r][c - 1].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Up, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r][c - 1].pos, { r, c - 1 });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
		stage1[r][c].doors.push_back(door);
		door->OnDebug = [this, door]()
		{
			if (isDebug)
			{
				door->col.setOutlineColor(sf::Color::Green);
			}
			else
			{
				door->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}
	if (stage1[r][c + 1].tag != NULL && c < 8)
	{
		std::string doorPath = (stage1[r][c + 1].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Down, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r][c + 1].pos, { r, c + 1 });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
		stage1[r][c].doors.push_back(door);
		door->OnDebug = [this, door]()
		{
			if (isDebug)
			{
				door->col.setOutlineColor(sf::Color::Green);
			}
			else
			{
				door->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}
	if (stage1[r - 1][c].tag != NULL && r > 0)
	{
		std::string doorPath = (stage1[r - 1][c].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Left, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r - 1][c].pos, { r - 1, c });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
		stage1[r][c].doors.push_back(door);
		door->OnDebug = [this, door]()
		{
			if (isDebug)
			{
				door->col.setOutlineColor(sf::Color::Green);
			}
			else
			{
				door->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}
	if (stage1[r + 1][c].tag != NULL && r < 8)
	{
		std::string doorPath = (stage1[r + 1][c].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Right, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r + 1][c].pos, { r + 1, c });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
		stage1[r][c].doors.push_back(door);
		door->OnDebug = [this, door]()
		{
			if (isDebug)
			{
				door->col.setOutlineColor(sf::Color::Green);
			}
			else
			{
				door->col.setOutlineColor(sf::Color::Transparent);
			}
		};
	}
}
void SceneGame::RandomRooms()
{
	int r = 0;
	int c = 0;

	do
	{
		r = Utils::RandomRange(1, 9) - 1;
		c = Utils::RandomRange(1, 9) - 1;
	} while (stage1[r][c].tag != NULL);

	stage1[r][c].pos = { 0.0f, 0.0f };
	CallRoom("room/Spawn.csv", { 0.0f, 0.0f }, r, c);

	int count = 0;
	int maxcount = 7;

	enum class Dir
	{
		Up,
		Right,
		Down,
		Left,
	};

	while (count <= maxcount)
	{
		do
		{
			r = Utils::RandomRange(1, 9) - 1;
			c = Utils::RandomRange(1, 9) - 1;
		} while (stage1[r][c].tag == NULL);

		Dir d = (Dir)Utils::RandomRange(0, 3);
		std::string randomPath = "room/Room" + std::to_string(Utils::RandomRange(1, 9)) + ".csv";
		if (count == maxcount) randomPath = "room/Boss1.csv";

		switch (d)
		{
		case Dir::Up:
			if (c - 1 < 0) break; //배열 최대 범위
			if (stage1[r][c - 1].tag != NULL) break; //존재하면 다시

			CallRoom(randomPath, { stage1[r][c].pos.x, stage1[r][c].pos.y - ROOM_INTERVAL }, r, c - 1);
			c--;
			count++;

			break;
		case Dir::Right:
			if (r + 1 > 8) break;
			if (stage1[r + 1][c].tag != NULL) break;

			CallRoom(randomPath, { stage1[r][c].pos.x + ROOM_INTERVAL, stage1[r][c].pos.y }, r + 1, c);
			r++;
			count++;

			break;
		case Dir::Down:
			if (c + 1 > 8) break;
			if (stage1[r][c + 1].tag != NULL) break;

			CallRoom(randomPath, { stage1[r][c].pos.x, stage1[r][c].pos.y + ROOM_INTERVAL }, r, c + 1);
			c++;
			count++;

			break;
		case Dir::Left:
			if (r - 1 < 0) break;
			if (stage1[r - 1][c].tag != NULL) break;
			CallRoom(randomPath, { stage1[r][c].pos.x - ROOM_INTERVAL, stage1[r][c].pos.y }, r - 1, c);
			r--;
			count++;

			break;
		}

	}
	stage1[r][c].tag = 'B';
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
SpriteGameObject* SceneGame::LoadObj(ObjType objtype, const std::string& textureId, const sf::FloatRect& wall, int r, int c)
{
	switch (objtype)
	{
	case ObjType::None:
	{
		SpriteGameObject* none = (SpriteGameObject*)AddGO(new SpriteGameObject(textureId));
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
		std::string poopPath = "graphics/poop" + std::to_string(Utils::RandomRange(1, 3)) + ".png";
		RoomObject* poop = (RoomObject*)AddGO(new RoomObject(poopPath));
		poop->sprite.setTextureRect(sf::IntRect{0, 0, 58, 65});
		poop->SetMaxHp(4);
		poop->SetPlayer(player);
		poop->OnHit = [poop](int damage)
		{
			poop->OnDamage(damage);

			switch (poop->GetHp())
			{
			case 3:
				poop->sprite.setTextureRect(sf::IntRect{58, 0, 58, 65});
				break;
			case 2:
				poop->sprite.setTextureRect(sf::IntRect{116, 0, 58, 65});
				break;
			case 1:
				poop->sprite.setTextureRect(sf::IntRect{174, 0, 58, 65});
				break;
			case 0:
				poop->sprite.setTextureRect(sf::IntRect{232, 0, 58, 65});
				break;
			}
		};
		poop->OnDie = [this, poop]()
		{
			poop->sortLayer = 0;
			poop->OnBump = nullptr;
			poop->OnHit = nullptr;
			hitablelist.remove(poop);
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
		Monster* attackfly = (Monster*)AddGO(new Monster(objtype, r, c));
		attackfly->SetPlayer(player);
		attackfly->SetMonster(1, 150.0f, 3, 500.0f);
		attackfly->OnBump = [this, attackfly]()
		{
			player->OnHit(attackfly->GetDamage());
		};
		attackfly->SetWall(wall);
		hitablelist.push_back(attackfly);
		stage1[r][c].monsters.push_back(attackfly);
		return (SpriteGameObject*)attackfly;
	}
	break;
	case ObjType::Pooter:
	{
		Monster* pooter = (Monster*)AddGO(new Monster(objtype, r, c));
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
		stage1[r][c].monsters.push_back(pooter);
		return (SpriteGameObject*)pooter;
	}
	break;
	case ObjType::Sucker:
	{
		Monster* sucker = (Monster*)AddGO(new Monster(objtype, r, c));
		sucker->SetPlayer(player);
		sucker->SetMonster(1, 100.0f, 5, 400.0f);
		sucker->OnBump = [this, sucker]()
		{
			player->OnHit(sucker->GetDamage());
		};
		sucker->OnDie = [this, sucker, wall, r, c]()
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

			RemoveMonster(sucker, r, c);
		};
		sucker->SetWall(wall);
		hitablelist.push_back(sucker);
		stage1[r][c].monsters.push_back(sucker);
		return (SpriteGameObject*)sucker;
	}
	break;
	case ObjType::DukeOfFlies:
	{
		Boss* duke = (Boss*)AddGO(new Boss(objtype, r, c, textureId));
		duke->SetPlayer(player);
		duke->SetMonster(1, 50.0f, 50, 800.0f);
		duke->OnBump = [this, duke]()
		{
			player->SetPosition(player->GetPosition() - Utils::Normalize(duke->GetPosition() - player->GetPosition()));
			player->OnHit(duke->GetDamage());
		};
		duke->Pattern1 = [this, duke, wall, r, c]()
		{
			if (Utils::Distance(duke->GetPosition(), player->GetPosition()) > 200.0f
				|| stage1[r][c].monsters.size() > 6)
			{
				return false;
			}
			for (int i = 0; i < 2; i++)
			{
				Monster* attackfly = (Monster*)AddGO(new Monster(ObjType::AttackFly, r, c));
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
				attackfly->Init();
				attackfly->Reset();
				hitablelist.push_back(attackfly);
				stage1[r][c].monsters.push_back(attackfly);
			}
			
			return true;
		};
		duke->Pattern2 = [this, duke, wall, r, c]()
		{
			if (Utils::Distance(duke->GetPosition(), player->GetPosition()) >  700.0f
				|| stage1[r][c].monsters.size() > 6)
			{
				return false;
			}
			for (int i = 0; i < 2; i++)
			{
				Monster* pooter = (Monster*)AddGO(new Monster(ObjType::Pooter, r, c));
				pooter->SetPlayer(player);
				pooter->SetMonster(1, 100.0f, 4, 400.0f);
				pooter->OnBump = [this, pooter]()
				{
					player->OnHit(1);
				};
				pooter->BloodShoot = [this, pooter, wall]()
				{
					Blood* blood = poolBloods.Get();
					blood->SetWall(wall);
					blood->Shoot(pooter->GetPosition(), pooter->GetDirection(), 300.0f, 1);
					AddGO(blood);
				};
				pooter->SetWall(wall);
				pooter->SetOrigin(Origins::C);
				pooter->SetPosition(duke->GetPosition().x - 100.0f + (200.0f * i), duke->GetPosition().y);
				pooter->sortLayer = 1;
				pooter->sortOrder = 1;
				pooter->Init();
				pooter->Reset();
				hitablelist.push_back(pooter);
				stage1[r][c].monsters.push_back(pooter);
			}

			return true;
		};
		duke->SetCooltime(2.0f, 5.0f);
		duke->SetWall(wall);
		hitablelist.push_back(duke);
		stage1[r][c].monsters.push_back(duke);
		return (SpriteGameObject*)duke;
	}
	default:
	{
		Tile* tile = (Tile*)AddGO(new Tile(objtype, textureId));
		return (SpriteGameObject*)tile;
	}
	break;
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
void SceneGame::RemoveMonster(Monster* monster, int r, int c)
{
	stage1[r][c].monsters.remove(monster);
	hitablelist.remove(monster);
	RemoveGO(monster);

	DoorControl(r, c);
}
void SceneGame::DoorControl(int r, int c)
{
	if (stage1[r][c].monsters.empty())
	{
		for (auto it : stage1[r][c].doors)
		{
			it->Open();
		}

		int per = Utils::RandomRange(1, 100);
		if (per >= 50) // 50%
		{
			Pill* pill = (Pill*)AddGO(new Pill());
			pill->SetType((Pill::PillType)Utils::RandomRange(0, (int)Pill::PillType::Count - 1));
			pill->SetPlayer(player);
			pill->SetPosition(stage1[r][c].pos);
			pill->OnDebug = [this, pill]()
			{
				if (isDebug)
				{
					pill->col.setOutlineColor(sf::Color::Green);
				}
				else
				{
					pill->col.setOutlineColor(sf::Color::Transparent);
				}
			};
			pill->Init();
			pill->Reset();
		}
	}
}
void SceneGame::OnDiePlayer()
{
	isAlive = false;

	SpriteGameObject* gameover = (SpriteGameObject*)FindGO("gameover");
	gameover->SetActive(true);
}