#pragma once
#include "SpriteGameObject.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class EffectObject : public SpriteGameObject
{
protected:
	AnimationController animation;

	float timer = 0.0f;
	float duration = 0.8f;
	std::string anim;

	sf::Sound splashsound;
public:
	ObjectPool<EffectObject>* pool;

	EffectObject(const std::string& textureId = "", const std::string& name = "");
	virtual ~EffectObject() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override { }

	void SetAnimation(const std::string& anim);
};