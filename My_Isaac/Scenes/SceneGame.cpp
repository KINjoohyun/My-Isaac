#include "stdafx.h"
#include "SceneGame.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Framework.h"
#include "DataTableMgr.h"
#include "StringTable.h"
#include "Variables.h"
#include "VertexArrayGameObject.h"

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
	uiView.setCenter({ windowSize.x / 2.0f, windowSize.y / 2.0f });

	sf::Vector2f tileSize = { 500.0f, 300.0f };
	sf::Vector2f textureSize = { 234.0f, 156.0f };

	VertexArrayGameObject* background = CreateBackground({ 2, 2 }, tileSize, textureSize, "graphics/basement.png");
	background->SetPosition(0.0f, 0.0f);
	background->SetOrigin(Origins::C);
	background->sortLayer = -1;
	AddGO(background);

	player = (Player*)AddGO(new Player());

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

VertexArrayGameObject* SceneGame::CreateBackground(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f textureSize, std::string textureId)
{
	VertexArrayGameObject* background = new VertexArrayGameObject(textureId);

	background->vertexArray.setPrimitiveType(sf::Quads);
	background->vertexArray.resize(size.x * size.y * 4);

	sf::Vector2f startPos = { 0.0f, 0.0f };
	sf::Vector2f tileOffset[4] =
	{
		{ 0.0f, 0.0f },
		{ tileSize.x, 0.0f },
		{ tileSize.x, tileSize.y },
		{ 0.0f, tileSize.y }
	};
	sf::Vector2f textureOffset[4] =
	{
		{ 0.0f, 0.0f },
		{ textureSize.x, 0.0f },
		{ textureSize.x, textureSize.y },
		{ 0.0f, textureSize.y }
	};

	// Hard Coding -> 수식화 필요
	background->vertexArray[0].position = startPos + tileOffset[0];
	background->vertexArray[0].texCoords = textureOffset[0];
	background->vertexArray[1].position = startPos + tileOffset[1];
	background->vertexArray[1].texCoords = textureOffset[1];
	background->vertexArray[2].position = startPos + tileOffset[2];
	background->vertexArray[2].texCoords = textureOffset[2];
	background->vertexArray[3].position = startPos + tileOffset[3];
	background->vertexArray[3].texCoords = textureOffset[3];

	background->vertexArray[4].position = startPos + tileOffset[0] + sf::Vector2f{ 0.0f, tileSize.y };
	background->vertexArray[4].texCoords = textureOffset[3];
	background->vertexArray[5].position = startPos + tileOffset[1] + sf::Vector2f{ 0.0f, tileSize.y };
	background->vertexArray[5].texCoords = textureOffset[2];
	background->vertexArray[6].position = startPos + tileOffset[2] + sf::Vector2f{ 0.0f, tileSize.y };
	background->vertexArray[6].texCoords = textureOffset[1];
	background->vertexArray[7].position = startPos + tileOffset[3] + sf::Vector2f{ 0.0f, tileSize.y };
	background->vertexArray[7].texCoords = textureOffset[0];

	background->vertexArray[8].position = startPos + tileOffset[0] + sf::Vector2f{ tileSize.x, 0.0f };
	background->vertexArray[8].texCoords = textureOffset[1];
	background->vertexArray[9].position = startPos + tileOffset[1] + sf::Vector2f{ tileSize.x, 0.0f };
	background->vertexArray[9].texCoords = textureOffset[0];
	background->vertexArray[10].position = startPos + tileOffset[2] + sf::Vector2f{ tileSize.x, 0.0f };
	background->vertexArray[10].texCoords = textureOffset[3];
	background->vertexArray[11].position = startPos + tileOffset[3] + sf::Vector2f{ tileSize.x, 0.0f };
	background->vertexArray[11].texCoords = textureOffset[2];

	background->vertexArray[12].position = startPos + tileOffset[0] + sf::Vector2f{ tileSize.x, tileSize.y };
	background->vertexArray[12].texCoords = textureOffset[2];
	background->vertexArray[13].position = startPos + tileOffset[1] + sf::Vector2f{ tileSize.x, tileSize.y };
	background->vertexArray[13].texCoords = textureOffset[3];
	background->vertexArray[14].position = startPos + tileOffset[2] + sf::Vector2f{ tileSize.x, tileSize.y };
	background->vertexArray[14].texCoords = textureOffset[0];
	background->vertexArray[15].position = startPos + tileOffset[3] + sf::Vector2f{ tileSize.x, tileSize.y };
	background->vertexArray[15].texCoords = textureOffset[1];

	return background;
}