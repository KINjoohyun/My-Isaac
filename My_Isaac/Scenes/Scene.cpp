#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "Framework.h"

Scene::Scene(SceneId id) :sceneId(id), window(FRAMEWORK.GetWindow())
{

}
Scene::~Scene()
{
	
}

void Scene::Update(float dt)
{
	for (auto go : gameObjects)
	{
		go->Update(dt);
	}

	for (auto rgo : removeGameObjects)
	{
		gameObjects.remove(rgo);
	}
	removeGameObjects.clear();
}
void Scene::Draw(sf::RenderWindow& window)
{
	SortGOs();

	window.setView(worldView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100) continue; // 100 이상의 Layer는 UI

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}

	window.setView(uiView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer < 100) continue; // UI

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}

void Scene::Enter()
{
	RESOURCE_MGR.LoadFormCSV(resourceListPath);

	for (auto go : gameObjects)
	{
		go->Reset();
	}
}
void Scene::Exit()
{
	for (auto rgo : removeGameObjects)
	{
		gameObjects.remove(rgo);
	}
	removeGameObjects.clear();

	RESOURCE_MGR.UnLoadAll();
}

GameObject* Scene::FindGO(const std::string& name)
{
	auto compare = [name](GameObject* go)
	{
		return go->GetName() == name;
	};
	auto it = std::find_if(gameObjects.begin(), gameObjects.end(), compare);
	if (it == gameObjects.end())
		return nullptr;
	return *it;

	/*
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}
	return nullptr;
	*/
}
void Scene::FindGOs(std::list<GameObject*>& list, const std::string& name)
{
	//list.clear();
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			list.push_back(go);
		}
	}
}
bool Scene::Exist(GameObject* go)
{
	return std::find(gameObjects.begin(), gameObjects.end(), go) != gameObjects.end();
}
GameObject* Scene::AddGO(GameObject* go)
{
	if (!Exist(go))
	{
		gameObjects.push_back(go);
		return go;
	}
	return nullptr;
}
void Scene::RemoveGO(GameObject* go)
{
	removeGameObjects.push_back(go);
	//gameObjects.remove(go);
	//delete go;
}
void Scene::SortGOs()
{
	gameObjects.sort([](GameObject* lhs, GameObject* rhs)
		{
			if (lhs->sortLayer != rhs->sortLayer)
				return lhs->sortLayer < rhs->sortLayer;
			return lhs->sortOrder < rhs->sortOrder;
		});
}

sf::Vector2f Scene::ScreenToWorldPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, worldView);
}
sf::Vector2f Scene::ScreenToUiPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, uiView);
}
sf::Vector2f Scene::WolrdPosToScreen(sf::Vector2f worldPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(worldPos, worldView);
}
sf::Vector2f Scene::UiPosToScreen(sf::Vector2f uiPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(uiPos, worldView);
}