#pragma once
#include "Monster.h"

class Boss : public Monster
{
protected:
	float p1Timer = 0.0f;
	float p1Cooltime = 0.0f;

	float p2Timer = 0.0f;
	float p2Cooltime = 0.0f;

	float p3Timer = 0.0f;
	float p3Cooltime = 0.0f;
public:
	Boss(ObjType objtype, const std::string& textureId = "", const std::string& name = "");
	virtual ~Boss() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override;

	void SetCooltime(float p1Cooltime = 0.0f, float p2Cooltime = 0.0f, float p3Cooltime = 0.0f);

	std::function<bool()>Pattern1;
	std::function<bool()>Pattern2;
	std::function<bool()>Pattern3;
};