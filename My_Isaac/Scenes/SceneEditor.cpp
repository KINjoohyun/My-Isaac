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

	SetTools();

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

void SceneEditor::SetTools()
{
	SpriteGameObject* background = (SpriteGameObject*)AddGO(new SpriteGameObject("graphics/editscreen.png"));
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
	saveButton->OnClick = [this]()
	{
		if (roomName == "")
		{
			std::cerr << "ERROR: Can't Save Room!" << std::endl;
			return;
		}
		RoomSave(roomName);
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
		if (roomName == "")
		{
			std::cerr << "ERROR: Can't Load Room!" << std::endl;
			return;
		}
		RoomReset();
		RoomLoad(roomName);
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
		rapidcsv::Document doc("room/BG1.csv");
		std::string bg = doc.GetColumn<std::string>(0).front();
		SetBackground(bg);

		sizex = doc.GetCell<int>(1, 0);
		sizey = doc.GetCell<int>(2, 0);
		gridx = doc.GetCell<int>(3, 0);
		gridy = doc.GetCell<int>(4, 0);
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
		rapidcsv::Document doc("room/BG2.csv");
		std::string bg = doc.GetColumn<std::string>(0).front();
		SetBackground(bg);

		sizex = doc.GetCell<int>(1, 0);
		sizey = doc.GetCell<int>(2, 0);
		gridx = doc.GetCell<int>(3, 0);
		gridy = doc.GetCell<int>(4, 0);
		SetGrid(sizex, sizey, gridx, gridy);
	};
	bg2->sortLayer = 100;

	for (int i = 1; i <= 9; i++)
	{
		std::string path;
		path = "room/Room" + std::to_string(i) + ".csv";

		UITextButton* roomButton = (UITextButton*)AddGO(new UITextButton("fonts/DNFBitBitOTF.otf", path));
		roomButton->SetOrigin(Origins::C);
		roomButton->SetText(path, 12, sf::Color::White, 1.0f);
		roomButton->SetPosition(windowSize.x - 60.0f, windowSize.y * 0.15f + (windowSize.y * 0.05f * i));
		roomButton->OnEnter = []()
		{

		};
		roomButton->OnExit = []()
		{

		};
		roomButton->OnClick = [&, path]()
		{
			roomName = path;
		};
		roomButton->sortLayer = 100;
	}
	
	UIImageButton* rocks1 = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_rocks1.png"));
	rocks1->SetOrigin(Origins::C);
	rocks1->SetPosition(30.0f, windowSize.y * 0.1f);
	rocks1->sprite.setScale(0.5f, 0.5f);
	rocks1->OnClick = [this]()
	{
		Tile* rocks1 = (Tile*)AddGO(new Tile(ObjType::Rock, "graphics/grid_rocks1.png"));
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

	UIImageButton* poop1 = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_poop1.png"));
	poop1->SetOrigin(Origins::C);
	poop1->SetPosition(60.0f, windowSize.y * 0.1f);
	poop1->sprite.setScale(0.5f, 0.5f);
	poop1->OnClick = [this]()
	{
		Tile* poop1 = (Tile*)AddGO(new Tile(ObjType::Poop, "graphics/grid_poop1.png"));
		poop1->SetOrigin(Origins::C);
		poop1->SetPosition(0.0f, 0.0f);
		poop1->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		poop1->sortLayer = 1;
		poop1->sortOrder = 0;
		poop1->Init();
		poop1->Reset();
		currentRoom.push_back(poop1);
	};
	poop1->sortLayer = 100;

	UIImageButton* spikes1 = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_spikes1.png"));
	spikes1->SetOrigin(Origins::C);
	spikes1->SetPosition(90.0f, windowSize.y * 0.1f);
	spikes1->sprite.setScale(0.5f, 0.5f);
	spikes1->OnClick = [this]()
	{
		Tile* spikes1 = (Tile*)AddGO(new Tile(ObjType::Spike, "graphics/grid_spikes1.png"));
		spikes1->SetOrigin(Origins::C);
		spikes1->SetPosition(0.0f, 0.0f);
		spikes1->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		spikes1->sortLayer = 1;
		spikes1->sortOrder = 0;
		spikes1->Init();
		spikes1->Reset();
		currentRoom.push_back(spikes1);
	};
	spikes1->sortLayer = 100;

	UIImageButton* attackfly = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_AttackFly.png"));
	attackfly->SetOrigin(Origins::C);
	attackfly->SetPosition(30.0f, windowSize.y * 0.2f);
	attackfly->sprite.setScale(0.5f, 0.5f);
	attackfly->OnClick = [this]()
	{
		Tile* attackfly = (Tile*)AddGO(new Tile(ObjType::AttackFly, "graphics/grid_AttackFly.png"));
		attackfly->SetOrigin(Origins::C);
		attackfly->SetPosition(0.0f, 0.0f);
		attackfly->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		attackfly->sortLayer = 1;
		attackfly->sortOrder = 0;
		attackfly->Init();
		attackfly->Reset();
		currentRoom.push_back(attackfly);
	};
	attackfly->sortLayer = 100;

	UIImageButton* pooter = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_Pooter.png"));
	pooter->SetOrigin(Origins::C);
	pooter->SetPosition(60.0f, windowSize.y * 0.2f);
	pooter->sprite.setScale(0.5f, 0.5f);
	pooter->OnClick = [this]()
	{
		Tile* pooter = (Tile*)AddGO(new Tile(ObjType::Pooter, "graphics/grid_Pooter.png"));
		pooter->SetOrigin(Origins::C);
		pooter->SetPosition(0.0f, 0.0f);
		pooter->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		pooter->sortLayer = 1;
		pooter->sortOrder = 0;
		pooter->Init();
		pooter->Reset();
		currentRoom.push_back(pooter);
	};
	pooter->sortLayer = 100;

	UIImageButton* sucker = (UIImageButton*)AddGO(new UIImageButton("graphics/grid_Sucker.png"));
	sucker->SetOrigin(Origins::C);
	sucker->SetPosition(90.0f, windowSize.y * 0.2f);
	sucker->sprite.setScale(0.5f, 0.5f);
	sucker->OnClick = [this]()
	{
		Tile* sucker = (Tile*)AddGO(new Tile(ObjType::Sucker, "graphics/grid_Sucker.png"));
		sucker->SetOrigin(Origins::C);
		sucker->SetPosition(0.0f, 0.0f);
		sucker->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		sucker->sortLayer = 1;
		sucker->sortOrder = 0;
		sucker->Init();
		sucker->Reset();
		currentRoom.push_back(sucker);
	};
	sucker->sortLayer = 100;
}
void SceneEditor::SetBackground(const std::string& texture)
{
	if (roomImage != nullptr)
	{
		RemoveGO(roomImage);
		roomImage = nullptr;
	}

	roomImage = (SpriteGameObject*)AddGO(new SpriteGameObject(texture, texture));
	roomImage->sprite.setColor({ 255, 255, 255, 200 });
	roomImage->SetOrigin(Origins::C);
	roomImage->SetPosition(0.0f, 0.0f);
	roomImage->sortLayer = 0;
	roomImage->Init();
	roomImage->Reset();
}
void SceneEditor::RoomReset()
{
	if (!currentRoom.empty())
	{
		for (auto it : currentRoom)
		{
			RemoveGO(it);
		}
	}
	currentRoom.clear();

	if (roomImage != nullptr)
	{
		RemoveGO(roomImage);
		roomImage = nullptr;
	}

	if (!grids.empty())
	{
		for (auto it : grids)
		{
			RemoveGO(it);
		}
	}
	grids.clear();
}
void SceneEditor::RoomSave(const std::string& roomPath)
{
	if (roomImage == nullptr)
	{
		std::cout << "WARNING: Not Exist roomImage" << std::endl;
		return;
	}

	rapidcsv::Document doc(roomPath, rapidcsv::LabelParams(-1, -1));
	doc.Clear();
	doc.SetCell<std::string>(0, 0, "BACKGROUND");
	doc.SetCell<std::string>(1, 0, "SIZEX");
	doc.SetCell<std::string>(2, 0, "SIZEY");
	doc.SetCell<std::string>(3, 0, "GRIDX");
	doc.SetCell<std::string>(4, 0, "GRIDY");

	doc.SetCell<std::string>(0, 3, "OBJ TYPE");
	doc.SetCell<std::string>(1, 3, "TEXTURE");
	doc.SetCell<std::string>(2, 3, "X");
	doc.SetCell<std::string>(3, 3, "Y");
	doc.SetCell<std::string>(4, 3, "ORDER");

	doc.SetCell<std::string>(0, 1, roomImage->GetName());
	doc.SetCell<int>(1, 1, sizex);
	doc.SetCell<int>(2, 1, sizey);
	doc.SetCell<int>(3, 1, gridx);
	doc.SetCell<int>(4, 1, gridy);

	int i = 4;
	for (auto it : currentRoom)
	{
		doc.SetCell<int>(0, i, (int)it->GetType());
		doc.SetCell<std::string>(1, i, it->GetTextureId());
		doc.SetCell<float>(2, i, it->GetPosition().x);
		doc.SetCell<float>(3, i, it->GetPosition().y);
		doc.SetCell<int>(4, i, it->sortOrder);

		i++;
	}
	doc.Save();
}
void SceneEditor::RoomLoad(const std::string& roomPath)
{
	rapidcsv::Document doc(roomPath, rapidcsv::LabelParams(-1, -1));
	std::string bg = doc.GetCell<std::string>(0, 1);
	SetBackground(bg);

	sizex = doc.GetCell<int>(1, 1);
	sizey = doc.GetCell<int>(2, 1);
	gridx = doc.GetCell<int>(3, 1);
	gridy = doc.GetCell<int>(4, 1);
	SetGrid(sizex, sizey, gridx, gridy);
	
	for (int i = 4; i < doc.GetRowCount(); i++)
	{
		auto rows = doc.GetRow<std::string>(i);
		Tile* obj = (Tile*)AddGO(new Tile((ObjType)std::stoi(rows[0]), rows[1]));
		obj->SetOrigin(Origins::C);
		obj->SetPosition(std::stof(rows[2]), std::stof(rows[3]));
		obj->order.setFont(*RESOURCE_MGR.GetFont("fonts/DNFBitBitOTF.otf"));
		obj->sortLayer = 1;
		obj->sortOrder = std::stoi(rows[4]);
		obj->Init();
		obj->Reset();
		currentRoom.push_back(obj);
	}
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
			grids.push_back(grid);
			AddGO(grid);
		}
	}
}

void SceneEditor::TileRemove(Tile* tile)
{
	currentRoom.remove(tile);
	RemoveGO(tile);
}