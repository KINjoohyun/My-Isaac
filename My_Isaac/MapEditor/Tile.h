#pragma once
#include "SpriteGameObject.h"

class Tile : public SpriteGameObject
{
protected:
	ObjType objtype;

	bool isHover = false;
public:
	sf::RectangleShape box;
	sf::Text order;

	Tile(const ObjType objtype, const std::string& textureId = "", const std::string& name = "");
	virtual ~Tile() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override {};

	void SetPosition(const sf::Vector2f& position);
	void SetPosition(float x, float y);

	void SetOrigin(Origins origin);
	void SetOrigin(float x, float y);
};