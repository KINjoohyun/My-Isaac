#pragma once
#include "Scene.h"

class SceneEditor : public Scene
{
protected:
	std::vector<std::string> textureBG;
public:
	SceneEditor();
	virtual ~SceneEditor() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;
};