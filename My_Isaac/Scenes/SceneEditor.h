#pragma once
#include "Scene.h"
#include "Tile.h"

class SpriteGameObject;

class SceneEditor : public Scene
{
protected:
	std::list<GameObject*> currentRoom;
	SpriteGameObject* roomImage = nullptr;
public:
	SceneEditor();
	virtual ~SceneEditor() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	void RoomReset();
	void RoomLoad(const std::string& roomPath);

	void TileRemove(GameObject* tile);
};