#include "stdafx.h"
#include "SceneGame.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Framework.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"

SceneGame::SceneGame() :Scene(SceneId::Game)
{
	
}
SceneGame::~SceneGame()
{
	Release();
}

void SceneGame::Init()
{
	Release();

	worldView.setSize(FRAMEWORK.GetWindowSize());

	uiView.setSize(FRAMEWORK.GetWindowSize());
	uiView.setCenter({ FRAMEWORK.GetWindowSize().x / 2.0f, FRAMEWORK.GetWindowSize().y / 2.0f });

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