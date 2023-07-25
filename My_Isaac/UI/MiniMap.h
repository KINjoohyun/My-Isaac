#pragma once
#include "SpriteGameObject.h"

class MiniMap : public SpriteGameObject
{
protected:
	sf::Sprite bossIcon;
	bool isOpenBoss = false;
public:
	sf::Sprite rooms[9][9];

	MiniMap(const std::string& textureId = "", const std::string& name = "") : SpriteGameObject(textureId, name) {}
	virtual ~MiniMap() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void SetPosition(const sf::Vector2f& position);

	virtual void SetOrigin(Origins origin);

	void RenewMiniMap();
	const bool IsNear(int i, int j) const;
};