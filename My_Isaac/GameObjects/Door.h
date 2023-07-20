#pragma once
#include "SpriteGameObject.h"

class Player;

class Door : public SpriteGameObject
{
public:
	enum class Look
	{
		Up,
		Right,
		Down,
		Left,
	};

protected:
	Look look;
	bool isOpen = true;

	Player* player;
	sf::Vector2f destination;

public:
	Door(const std::string& textureId = "", const sf::Vector2f& position = {0.0f, 0.0f}, Look look = Look::Up);
	virtual ~Door() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override { }
	
	void SetPlayer(Player* player);
	void SetDestination(const sf::Vector2f& destination);
	void Open();
	void Close();

	void Entrance();
};