#include "stdafx.h"
#include "SceneEditor.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SpriteGameObject.h"
#include "Framework.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "UIButton.h"

#include "rapidcsv.h"

SceneEditor::SceneEditor() : Scene(SceneId::Editor)
{
	resourceListPath = "scripts/SceneEditorResourceList.csv";
}
SceneEditor::~SceneEditor()
{
	Release();
}

void SceneEditor::Init()
{
	Release();

	worldView.setSize(windowSize);
	worldView.setCenter(0, 0);

	uiView.setSize(windowSize);
	uiView.setCenter({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	
	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/background.png"));
	background->SetOrigin(Origins::C);
	background->SetPosition(0.0f, 0.0f);
	background->sortLayer = -1;

	UIButton* saveButton = (UIButton*)AddGO(new UIButton("fonts/DNFBitBitOTF.otf"));
	saveButton->SetOrigin(Origins::R);
	saveButton->SetText("Save", 30, sf::Color::White, 1.0f);
	saveButton->SetPosition(windowSize.x, windowSize.y * 0.8f);
	saveButton->OnEnter = []()
	{
		
	};
	saveButton->OnExit = []()
	{
		
	};
	saveButton->OnClick = []()
	{
		std::cout << "save" << std::endl;
	};
	saveButton->sortLayer = 100;

	UIButton* loadButton = (UIButton*)AddGO(new UIButton("fonts/DNFBitBitOTF.otf"));
	loadButton->SetOrigin(Origins::R);
	loadButton->SetText("Load", 30, sf::Color::White, 1.0f);
	loadButton->SetPosition(windowSize.x, windowSize.y * 0.9f);
	loadButton->OnEnter = []()
	{
		
	};
	loadButton->OnExit = []()
	{

	};
	loadButton->OnClick = [this]()
	{
		//일단 하드 코딩 - room/Room1.csv
		rapidcsv::Document doc("room/Room1.csv");
		std::string bg = *doc.GetColumn<std::string>(0).begin();
		std::vector<int> types = doc.GetColumn<int>(1);
		std::vector<float> x = doc.GetColumn<float>(2);
		std::vector<float> y = doc.GetColumn<float>(3);

		for (int i = 0; i < types.size(); i++)
		{
			

			std::cout << i;
		}
		textureBG.push_back(bg);
		SpriteGameObject* back = (SpriteGameObject*)AddGO(new SpriteGameObject(bg));
		back->sprite.setColor({255, 255, 255, 200});
		back->SetOrigin(Origins::C);
		back->SetPosition(0.0f, 0.0f);
		back->sortLayer = 0;
		back->Init();
		back->Reset();

		std::cout << "load" << std::endl;
	};
	loadButton->sortLayer = 100;

	for (auto go : gameObjects)
	{
		go->Init();
	}
}
void SceneEditor::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneId::Title);
	}
}
void SceneEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
void SceneEditor::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneEditor::Enter()
{
	Scene::Enter();
}
void SceneEditor::Exit()
{
	Scene::Exit();
}
