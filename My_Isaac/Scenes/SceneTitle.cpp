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

#include "Player2.h"
#include "Player.h"
#include "RectGameObject.h"

#include "UIButton.h"

SceneTitle::SceneTitle() : Scene(SceneId::Title)
{
	resourceListPath = "scripts/SceneTitleResourceList.csv";
}

void SceneTitle::Init()
{
	Release();

	worldView.setSize(FRAMEWORK.GetWindowSize());
	worldView.setCenter(0, 0);

	uiView.setSize(FRAMEWORK.GetWindowSize());
	uiView.setCenter({ FRAMEWORK.GetWindowSize().x / 2.0f, FRAMEWORK.GetWindowSize().y / 2.0f });

	//Player2* player2 = (Player2*)AddGO(new Player2());
	//player2->sortLayer = 1;

	Player* player = (Player*)AddGO(new Player());
	player->sortLayer = 1;

	RectGameObject* rect = new RectGameObject();
	rect->rect.setSize({ 1000.0f, 100.0f });
	rect->rect.setFillColor(sf::Color::White);
	rect->rect.setOutlineColor(sf::Color::Red);
	rect->rect.setOutlineThickness(2);
	rect->SetOrigin(Origins::TC);
	AddGO(rect);

	UIButton* button = new UIButton("graphics/slide.png", "button");
	button->SetOrigin(Origins::C);
	button->SetPosition(FRAMEWORK.GetWindowSize().x / 2, FRAMEWORK.GetWindowSize().y);
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

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
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