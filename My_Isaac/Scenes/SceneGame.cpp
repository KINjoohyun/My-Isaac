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
#include "GuideObject.h"
#include "UITextButton.h"

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
	uiView.setCenter(windowSize * 0.5f);

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
	ui_bg->SetOrigin(Origins::TC);
	ui_bg->SetPosition({windowSize.x * 0.5f, 0.0f});
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

	poolBloods.OnCreate = [this](Blood* blood)
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
	pauseObject->SetPosition(windowSize* 0.5f);
	pauseObject->sortLayer = 100;
	pauseObject->SetActive(false);

	SpriteGameObject* gameover = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/gameover.png", "gameover"));
	gameover->SetOrigin(Origins::C);
	gameover->sortLayer = 100;
	gameover->SetPosition(windowSize * 0.5f);
	gameover->SetActive(false);

	SpriteGameObject* post = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/ui_post.png", "post"));
	post->SetOrigin(Origins::BR);
	post->sortLayer = 100;
	post->SetPosition({ windowSize.x, windowSize.y });
	post->SetActive(false);

	guide = (GuideObject*)AddGO(new GuideObject("graphics/ui/ui_guide.png", "STAGE 1"));
	guide->SetOrigin(Origins::C);
	guide->SetPosition({ windowSize.x * 0.5f, windowSize.y * 0.3f});
	guide->sortLayer = 101;

	for (auto go : gameObjects)
	{
		go->Init();
	}

	// Sound
}
void SceneGame::Update(float dt)
{
	fps++;
	fpstimer += dt;
	if (fpstimer >= 1.0f)
	{
		fpstext->text.setString("FPS : " + std::to_string(fps));
		fpstimer = 0.0f;
		fps = 0.0f;
	}

	if (!isPause)
	{
		Scene::Update(dt);
	}
	else
	{
		pauseObject->Update(dt);
	}
	timer += dt;

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::P) && isAlive)
	{
		isPause = !isPause;
		pauseObject->SetActive(isPause);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}

	// Debug Mode
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F5))
	{
		isDebug = !isDebug;
	}

	if (stage1[std::get<0>(bossroom)][std::get<1>(bossroom)].monsters.empty() && !stage1clear && isAlive)
	{
		stage1clear = true;

		PrintGameclear();
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
	lifebar.clear();
	hitablelist.clear();
}

void SceneGame::Enter()
{
	auto timertext = FindGO("timertext");
	RemoveGO(timertext);
	auto restart = FindGO("restart");
	RemoveGO(restart);
	auto exitgame = FindGO("exitgame");
	RemoveGO(exitgame);
	auto gameclear = FindGO("gameclear");
	RemoveGO(gameclear);

	RandomRooms();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SetDoor(i, j);
		}
	}

	Scene::Enter();

	fps = 0.0f;
	fpstimer = 0.0f;
	timer = 0.0f;
	isPause = false;
	isAlive = true;
	stage1clear = false;
	ClearPool(poolBloods);

	SpriteGameObject* gameover = (SpriteGameObject*)FindGO("gameover");
	gameover->SetActive(false);

	SpriteGameObject* post = (SpriteGameObject*)FindGO("post");
	post->SetActive(false);

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

	bgm.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/basement.ogg"));
	bgm.play();
	bgm.setLoop(true);
}
void SceneGame::Exit()
{
	isAlive = false;
	ClearRooms();
	for (auto it : hitablelist)
	{
		RemoveGO(it);
	}
	hitablelist.clear();
	for (auto it : lifebar)
	{
		RemoveGO(it);
	}
	lifebar.clear();
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
	objlist.push_back(background);

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
	objlist.push_back(wall);

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
		obj->Init();
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
		door->Init();
		stage1[r][c].doors.push_back(door);
	}
	if (stage1[r][c + 1].tag != NULL && c < 8)
	{
		std::string doorPath = (stage1[r][c + 1].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Down, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r][c + 1].pos, { r, c + 1 });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
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
		door->Init();
		stage1[r][c].doors.push_back(door);
	}
	if (stage1[r - 1][c].tag != NULL && r > 0)
	{
		std::string doorPath = (stage1[r - 1][c].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Left, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r - 1][c].pos, { r - 1, c });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
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
		door->Init();
		stage1[r][c].doors.push_back(door);
	}
	if (stage1[r + 1][c].tag != NULL && r < 8)
	{
		std::string doorPath = (stage1[r + 1][c].tag == 'B') ? "graphics/door_boss.png" : "graphics/door1.png";
		Door* door = (Door*)AddGO(new Door(doorPath, Door::Look::Right, {r, c}));
		door->SetPlayer(player);
		door->SetDestination(stage1[r + 1][c].pos, { r + 1, c });
		door->SetWall(stage1[r][c].wall);
		(stage1[r][c].monsters.empty()) ? door->Open() : door->Close();
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
		door->Init();
		stage1[r][c].doors.push_back(door);
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
	bossroom = {r, c};
}
void SceneGame::ClearRooms()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			stage1[i][j].tag = NULL;
			stage1[i][j].pos = {};
			stage1[i][j].wall = {};
			for (auto it : stage1[i][j].monsters)
			{
				RemoveGO(it);
			}
			stage1[i][j].monsters.clear();
			for (auto it : stage1[i][j].doors)
			{
				RemoveGO(it);
			}
			stage1[i][j].doors.clear();

			stage1[i][j].isHear = false;
			stage1[i][j].isPassed = false;
			stage1[i][j].isClarify = false;
		}
	}

	for (auto it : objlist)
	{
		RemoveGO(it);
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
SpriteGameObject* SceneGame::LoadObj(ObjType objtype, const std::string& textureId, const sf::FloatRect& wall, int r, int c)
{
	switch (objtype)
	{
	case ObjType::None:
	{
		SpriteGameObject* none = (SpriteGameObject*)AddGO(new SpriteGameObject(textureId));
		objlist.push_back(none);
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
		objlist.push_back(spike);
		return (SpriteGameObject*)spike;
	}
	break;
	case ObjType::AttackFly:
	{
		Monster* attackfly = (Monster*)AddGO(new Monster(objtype, r, c));
		attackfly->SetPlayer(player);
		attackfly->SetMonster(1, 150.0f, 3, 500.0f);
		attackfly->SetSound("sounds/Fly.ogg");
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
		pooter->SetSound("sounds/Fly.ogg");
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
		sucker->SetSound("sounds/Fly.ogg");
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
		duke->SetMonster(1, 30.0f, 50, 800.0f, true);
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
				attackfly->SetSound("sounds/Fly.ogg");
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
				attackfly->OnDebug = [this, attackfly]()
				{
					if (isDebug)
					{
						attackfly->col.setOutlineColor(sf::Color::Red);
					}
					else
					{
						attackfly->col.setOutlineColor(sf::Color::Transparent);
					}
				};
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
				pooter->SetSound("sounds/Fly.ogg");
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
				pooter->OnDebug = [this, pooter]()
				{
					if (isDebug)
					{
						pooter->col.setOutlineColor(sf::Color::Red);
					}
					else
					{
						pooter->col.setOutlineColor(sf::Color::Transparent);
					}
				};
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
		std::cout << "WARNING: Not Exist ObjType" << std::endl;
		Tile* tile = (Tile*)AddGO(new Tile(objtype, textureId));
		objlist.push_back(tile);
		return (SpriteGameObject*)tile;
	}
	break;
	}
}
const std::list<RoomObject*>* SceneGame::GetPoopList() const
{
	return &hitablelist;
}
void SceneGame::RemoveRoomGO(RoomObject* roomGO)
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
	bgm.setBuffer(*RESOURCE_MGR.GetSoundBuffer("sounds/you_died.ogg"));
	bgm.play();
	bgm.setLoop(false);

	SpriteGameObject* gameover = (SpriteGameObject*)FindGO("gameover");
	gameover->SetActive(true);

	SpriteGameObject* post = (SpriteGameObject*)FindGO("post");
	post->SetActive(true);

	PrintMenu();
}
void SceneGame::PrintMenu()
{
	TextGameObject* timertext = (TextGameObject*)AddGO(new TextGameObject("fonts/DNFBitBitOTF.otf", "timertext"));
	timertext->SetOrigin(Origins::C);
	timertext->SetPosition({ windowSize.x * 0.5f, windowSize.y * 0.35f });
	timertext->text.setString("TIME[ " + std::to_string((int)timer / 60) + " : " + std::to_string((int)timer % 60) + " ]");
	timertext->text.setOutlineColor(sf::Color::Black);
	timertext->text.setOutlineThickness(1);
	timertext->text.setCharacterSize(20);
	timertext->sortLayer = 105;
	timertext->Init();
	timertext->Reset();

	UITextButton* restart = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf", "restart"));
	restart->SetOrigin(Origins::C);
	restart->SetPosition({ windowSize.x * 0.875f, windowSize.y * 0.75f });
	restart->SetText("RESTART", 30);
	restart->OnEnter = []()
	{

	};
	restart->OnExit = []()
	{

	};
	restart->OnClick = [this]()
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	};
	restart->sortLayer = 105;
	restart->Init();
	restart->Reset();

	UITextButton* exitgame = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf", "exitgame"));
	exitgame->SetOrigin(Origins::C);
	exitgame->SetPosition({ windowSize.x * 0.875f, windowSize.y * 0.85f });
	exitgame->SetText("GO TITLE", 30);
	exitgame->OnEnter = []()
	{

	};
	exitgame->OnExit = []()
	{

	};
	exitgame->OnClick = [this]()
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	};
	exitgame->sortLayer = 105;
	exitgame->Init();
	exitgame->Reset();
}
void SceneGame::PrintGuide(const std::string& text)
{
	guide->SetString(text);
	guide->Reset();
}
void SceneGame::PrintGameclear()
{
	SpriteGameObject* gameclear = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/ui/gameclear.png", "gameclear"));
	gameclear->SetOrigin(Origins::C);
	gameclear->SetPosition(windowSize * 0.5f);
	gameclear->sortLayer = 105;
	gameclear->Init();
	gameclear->Reset();

	PrintMenu();
}
void SceneGame::SoundPlaying(const std::string& path)
{
	gamesound.setBuffer(*RESOURCE_MGR.GetSoundBuffer(path));
	gamesound.play();
}