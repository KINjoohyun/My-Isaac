#include "stdafx.h"
#include "SceneGame.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Framework.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "SpriteGameObject.h"

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

	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/basement_bg1.png", "bg"));
	background->SetOrigin(Origins::C);
	
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
	Scene::Exit();
}