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
	worldView.setCenter(0, 0);

	uiView.setSize(windowSize);
	uiView.setCenter({ windowSize.x * 0.5f, windowSize.y * 0.5f });

	// 하드 코딩으로 랜덤한 Room 호출
	std::string randomPath = "room/Room" + std::to_string(Utils::RandomRange(1, 9)) + ".csv";
	CallRoom(randomPath);

	player = (Player*)AddGO(new Player());
	player->sortLayer = 1;

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

	// 하드 코딩으로 벽 생성
	RectGameObject* wall = (RectGameObject*)FindGO("wall");
	float wallTop = wall->rect.getGlobalBounds().top;
	float wallBottom = wall->rect.getGlobalBounds().top + wall->rect.getGlobalBounds().height;
	float wallLeft = wall->rect.getGlobalBounds().left;
	float wallRight = wall->rect.getGlobalBounds().left + wall->rect.getGlobalBounds().width;

	if (!wall->rect.getGlobalBounds().contains(player->GetPosition()))
	{
		player->SetPosition(Utils::Clamp(player->GetPosition(), {wallLeft, wallTop}, {wallRight, wallBottom}));
	}
}
void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneGame::Enter()
{
	Scene::Enter();
}
void SceneGame::Exit()
{
	player->Reset();

	Scene::Exit();
}

void SceneGame::CallRoom(const std::string& roomPath)
{
	rapidcsv::Document doc(roomPath, rapidcsv::LabelParams(-1, -1));
	std::string bg = doc.GetCell<std::string>(0, 1);
	
	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject(bg, "bg"));
	background->SetOrigin(Origins::C);

	int sizex = doc.GetCell<int>(1, 1);
	int sizey = doc.GetCell<int>(2, 1);

	RectGameObject* wall = (RectGameObject*)AddGO(new RectGameObject("wall"));
	wall->rect.setSize({ (float)sizex, (float)sizey });
	wall->rect.setOutlineColor(sf::Color::Blue);
	wall->rect.setOutlineThickness(1);
	wall->rect.setFillColor(sf::Color::Transparent);
	wall->SetOrigin(Origins::C);
	wall->SetPosition(0.0f, 0.0f);

	for (int i = 4; i < doc.GetRowCount(); i++)
	{
		auto rows = doc.GetRow<std::string>(i);
		Tile* obj = (Tile*)AddGO(new Tile((ObjType)std::stoi(rows[0]), rows[1])); //현재는 Tile을 로드
		obj->SetOrigin(Origins::C);
		obj->SetPosition(std::stof(rows[2]), std::stof(rows[3]));
		obj->sortLayer = 1;
		obj->sortOrder = std::stoi(rows[4]);
		obj->Init();
		obj->Reset();
	}
}