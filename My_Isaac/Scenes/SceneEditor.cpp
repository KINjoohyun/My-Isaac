#include "stdafx.h"
#include "SceneEditor.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SpriteGameObject.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "UITextButton.h"
#include "UIImageButton.h"
#include "Tile.h"
#include "RectGameObject.h"

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

	UITextButton* resetButton = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	resetButton->SetOrigin(Origins::R);
	resetButton->SetText("Reset", 30, sf::Color::White, 1.0f);
	resetButton->SetPosition(windowSize.x, windowSize.y * 0.7f);
	resetButton->OnEnter = []()
	{

	};
	resetButton->OnExit = []()
	{

	};
	resetButton->OnClick = [this]()
	{
		RoomReset();
		std::cout << "Reset" << std::endl;
	};
	resetButton->sortLayer = 100;

	UITextButton* saveButton = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
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

	UITextButton* loadButton = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
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
		RoomReset();
		RoomLoad("room/Room1.csv");
	};
	loadButton->sortLayer = 100;

	UITextButton* bg1 = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	bg1->SetOrigin(Origins::R);
	bg1->SetText("BG1", 20, sf::Color::White, 1.0f);
	bg1->SetPosition(windowSize.x - 70.0f, windowSize.y * 0.1f);
	bg1->OnEnter = []()
	{

	};
	bg1->OnExit = []()
	{

	};
	bg1->OnClick = [this]()
	{
		RoomReset();

		rapidcsv::Document doc("room/BG1.csv");
		std::string bg = doc.GetColumn<std::string>(0).front();
		SetBackground(bg);

		int sizex = doc.GetCell<int>(1, 0);
		int sizey = doc.GetCell<int>(1, 1);
		int gridx = doc.GetCell<int>(2, 0);
		int gridy = doc.GetCell<int>(2, 1);
		SetGrid(sizex, sizey, gridx, gridy);
	};
	bg1->sortLayer = 100;

	UITextButton* bg2 = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf"));
	bg2->SetOrigin(Origins::R);
	bg2->SetText("BG2", 20, sf::Color::White, 1.0f);
	bg2->SetPosition(windowSize.x - 10.0f, windowSize.y * 0.1f);
	bg2->OnEnter = []()
	{

	};
	bg2->OnExit = []()
	{

	};
	bg2->OnClick = [this]()
	{
		RoomReset();

		rapidcsv::Document doc("room/BG2.csv");
		std::string bg = doc.GetColumn<std::string>(0).front();
		SetBackground(bg);
		
		int sizex = doc.GetCell<int>(1, 0);
		int sizey = doc.GetCell<int>(1, 1);
		int gridx = doc.GetCell<int>(2, 0);
		int gridy = doc.GetCell<int>(2, 1);
		SetGrid(sizex, sizey, gridx, gridy);
	};
	bg2->sortLayer = 100;

	UIImageButton* rocks1 = (UIImageButton*)AddGO(new UIImageButton("graphics/rocks1.png"));
	rocks1->SetOrigin(Origins::C);
	rocks1->SetPosition(30.0f, windowSize.y * 0.1f);
	rocks1->sprite.setScale(0.5f, 0.5f);
	rocks1->OnClick = [this]()
	{
		Tile* rocks1 = (Tile*)AddGO(new Tile(ObjType::Rock, "graphics/rocks1.png"));
		rocks1->SetOrigin(Origins::C);
		rocks1->SetPosition(0.0f, 0.0f);
		rocks1->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		rocks1->sortLayer = 1;
		rocks1->sortOrder = 0;
		rocks1->Init();
		rocks1->Reset();
		currentRoom.push_back(rocks1);
	};
	rocks1->sortLayer = 100;

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

	RoomReset();
}
void SceneEditor::Exit()
{
	Scene::Exit();
}

void SceneEditor::SetBackground(const std::string& texture)
{
	roomImage = (SpriteGameObject*)AddGO(new SpriteGameObject(texture));
	roomImage->sprite.setColor({ 255, 255, 255, 200 });
	roomImage->SetOrigin(Origins::C);
	roomImage->SetPosition(0.0f, 0.0f);
	roomImage->sortLayer = 0;
	roomImage->Init();
	roomImage->Reset();
	currentRoom.push_back(roomImage);
}
void SceneEditor::RoomReset()
{
	if (currentRoom.empty()) return;

	for (auto it : currentRoom)
	{
		RemoveGO(it);
	}

	currentRoom.clear();
	if (roomImage != nullptr)
	{
		RemoveGO(roomImage);
		roomImage = nullptr;
	}
}
void SceneEditor::RoomLoad(const std::string& roomPath)
{
	//일단 하드 코딩
	rapidcsv::Document doc(roomPath);
	std::string bg = doc.GetColumn<std::string>(0).front();
	SetBackground(bg);

	int sizex = doc.GetCell<int>(1, 0);
	int sizey = doc.GetCell<int>(1, 1);
	int gridx = doc.GetCell<int>(2, 0);
	int gridy = doc.GetCell<int>(2, 1);
	SetGrid(sizex, sizey, gridx, gridy);
	
	std::vector<int> objtypes = doc.GetColumn<int>(3);
	std::vector<std::string> texture = doc.GetColumn<std::string>(4);
	std::vector<float> x = doc.GetColumn<float>(5);
	std::vector<float> y = doc.GetColumn<float>(6);
	std::vector<int> order = doc.GetColumn<int>(7);
	for (int i = 0; i < objtypes.size(); i++)
	{
		//objtypes 사용
		Tile* obj = (Tile*)AddGO(new Tile((ObjType)objtypes[i], texture[i]));
		obj->SetOrigin(Origins::C);
		obj->SetPosition(x[i], y[i]);
		obj->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		obj->sortLayer = 1;
		obj->sortOrder = order[i];
		obj->Init();
		obj->Reset();
		currentRoom.push_back(obj);
	}

	
	
	std::cout << roomPath << std::endl;
}
void SceneEditor::SetGrid(int sizex, int sizey, int r, int c)
{
	float width = sizex / r;
	float height = sizey / c;

	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			RectGameObject* grid = new RectGameObject();
			grid->rect.setSize({ width, height });
			grid->SetPosition(-sizex/2 + width * i, -sizey/2 + height * j);
			grid->rect.setOutlineColor(sf::Color::Black);
			grid->rect.setOutlineThickness(1);
			grid->rect.setFillColor(sf::Color::Transparent);
			currentRoom.push_back(grid);
			AddGO(grid);
		}
	}
}

void SceneEditor::TileRemove(GameObject* tile)
{
	currentRoom.remove(tile);
	RemoveGO(tile);
}