#pragma once
#include "Scene.h"
#include "Tile.h"

class SpriteGameObject;
class RectGameObject;
class TextBox;

class SceneEditor : public Scene
{
protected:
	std::list<Tile*> currentRoom;
	SpriteGameObject* roomImage = nullptr;
	std::string roomName = "";

	TextBox* roomTB = nullptr;

	int sizex = 0;
	int sizey = 0;
	int gridx = 0;
	int gridy = 0;
	std::vector<RectGameObject*> grids;

public:
	SceneEditor();
	virtual ~SceneEditor() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	void SetTools();
	void SetBackground(const std::string& texture);
	void RoomReset();
	void RoomSave(const std::string& roomPath);
	void RoomLoad(const std::string& roomPath);
	void SetGrid(int sizex, int sizey, int r, int c);

	void TileRemove(Tile* tile);
	void TextBoxApply();
};