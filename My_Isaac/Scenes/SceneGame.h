#pragma once
#include "Scene.h"
#include "GameObject.h"

class Player;
class VertexArrayGameObject;

class SceneGame : public Scene
{
protected:

	Player* player;

public:
	SceneGame();
	virtual ~SceneGame() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;
};