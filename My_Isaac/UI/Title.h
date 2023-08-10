#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"

class Title : public SpriteGameObject
{
protected:
	AnimationController animation;

public:
	Title(const std::string& textureId = "", const std::string& name = "");
	virtual ~Title() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
};