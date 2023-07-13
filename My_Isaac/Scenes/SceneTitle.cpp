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
	uiView.setCenter({ windowSize.x / 2.0f, windowSize.y / 2.0f });

	UIButton* button = new UIButton("graphics/slide.png", "button");
	button->SetOrigin(Origins::C);
	button->SetPosition(windowSize.x / 2, windowSize.y / 2);
	button->OnEnter = [button]()
	{
		std::cout << "Enter" << std::endl;
		button->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/slide_off.png"));
	};
	button->OnExit = [button]()
	{
		std::cout << "Exit" << std::endl;
		button->sprite.setTexture(*RESOURCE_MGR.GetTexture(button->textureId));
	};
	button->OnClick = []()
	{
		std::cout << "Click" << std::endl;
		SCENE_MGR.ChangeScene(SceneId::Game);
	};
	button->sortLayer = 100;
	AddGO(button);

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