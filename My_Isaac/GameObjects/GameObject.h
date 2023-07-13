#pragma once
#include "Utils.h"

class GameObject
{
protected:
	std::string name;
	sf::Vector2f position;
	bool isActive = true;
	Origins origin;
public:
	int sortLayer = 0;
	int sortOrder = 0;

	GameObject(const std::string name);
	virtual ~GameObject();
	
	virtual void Init() = 0;
	virtual void Reset() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void Release() {}
	
	bool GetActive() const;
	void SetActive(bool active);

	std::string GetName();
	virtual void SetName(const std::string& name);

	sf::Vector2f GetPosition();
	virtual void SetPosition(const sf::Vector2f& position);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
};