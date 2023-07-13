#include "stdafx.h"
#include "SceneEditor.h"
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

SceneEditor::SceneEditor() : Scene(SceneId::Editor)
{
	resourceListPath = "";
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
}
void SceneEditor::Update(float dt)
{
	Scene::Update(dt);
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
