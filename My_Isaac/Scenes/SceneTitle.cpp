#include "stdafx.h"
#include "SceneTitle.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SpriteGameObject.h"
#include "TextGameObject.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "UITextButton.h"
#include "Title.h"

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

	SpriteGameObject* empty = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/emptyscreen.png", "empty"));
	empty->SetOrigin(Origins::C);
	empty->SetPosition(0.0f, 0.0f);
	empty->sortLayer = -1;

	SpriteGameObject* logo = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/logo.png", "logo"));
	logo->SetOrigin(Origins::TC);
	logo->SetPosition({ 0.0f, -windowSize.y * 0.5f + 50.0f });
	logo->sortLayer = 0;

	UITextButton* button1 = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	button1->SetOrigin(Origins::C);
	button1->SetText("GAME START", 50);
	button1->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f + 150.0f);
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

	UITextButton* button2 = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	button2->SetOrigin(Origins::C);
	button2->SetText("MAP EDITOR", 50);
	button2->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f + 250.0f);
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

	UITextButton* button3 = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	button3->SetOrigin(Origins::C);
	button3->SetText("EXIT", 50);
	button3->SetPosition(windowSize.x * 0.5f, windowSize.y * 0.5f + 350.0f);
	button3->OnEnter = []()
	{

	};
	button3->OnExit = []()
	{

	};
	button3->OnClick = [this]()
	{
		window.close();
	};
	button3->sortLayer = 100;

	Title* title = (Title*)AddGO(new Title());

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