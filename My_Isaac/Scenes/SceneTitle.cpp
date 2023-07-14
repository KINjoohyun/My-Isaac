#include "stdafx.h"
#include "SceneTitle.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SpriteGameObject.h"
#include "TextGameObject.h"
#include "Framework.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "UIButton.h"

SceneTitle::SceneTitle() : Scene(SceneId::Title)
{
	resourceListPath = "scripts/SceneTitleResourceList.csv";
}
SceneTitle::~SceneTitle()
{
	Release();
}

void SceneTitle::Init()
{
	Release();

	worldView.setSize(windowSize);
	worldView.setCenter(0, 0);

	uiView.setSize(windowSize);
	uiView.setCenter({ windowSize.x * 0.5f, windowSize.y * 0.5f });

	UIButton* button1 = (UIButton*)AddGO(new UIButton("fonts/DNFBitBitOTF.otf"));
	button1->SetOrigin(Origins::C);
	button1->SetText("Game Start", 50, sf::Color::White, 1.0f);
	button1->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f + 50.0f);
	button1->OnEnter = []()
	{
		
	};
	button1->OnExit = []()
	{
		
	};
	button1->OnClick = [this]()
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	};
	button1->sortLayer = 100;

	UIButton* button2 = (UIButton*)AddGO(new UIButton("fonts/DNFBitBitOTF.otf"));
	button2->SetOrigin(Origins::C);
	button2->SetText("Map Editor", 50, sf::Color::White, 1.0f);
	button2->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f + 150.0f);
	button2->OnEnter = []()
	{
		
	};
	button2->OnExit = []()
	{
		
	};
	button2->OnClick = [this]()
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	};
	button2->sortLayer = 100;

	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/background.png", "bg"));
	background->SetOrigin(Origins::C);
	background->SetPosition(0.0f, 0.0f);
	background->sortLayer = -1;

	for (auto go : gameObjects)
	{
		go->Init();
	}
}
void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
}
void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();
}
void SceneTitle::Exit()
{
	Scene::Exit();
}