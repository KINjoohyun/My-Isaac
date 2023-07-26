#pragma once
#include "SpriteGameObject.h"

class Player;

class Pill : public SpriteGameObject
{
public:
	enum class PillType
	{
		None = -1,
		FullHealth,
		HealthDown,
		HealthUp,
		SpeedDown,
		SpeedUp,

		Count,
	};
protected:
	Player* player;
	PillType type;

public:
	Pill(const std::string& textureId = "graphics/pill.png", const std::string& name = "")
		: SpriteGameObject(textureId, name) {}
	virtual ~Pill() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	void SetPlayer(Player* player);
	void SetType(PillType type);
};