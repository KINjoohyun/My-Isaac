#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class TearEffect : public SpriteGameObject
{
protected:
	AnimationController animation;

	float timer = 0.0f;
	float duration = 0.8f;
public:
	ObjectPool<TearEffect>* pool;

	TearEffect(const std::string& textureId = "", const std::string& name = "");
	virtual ~TearEffect() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override { }
};