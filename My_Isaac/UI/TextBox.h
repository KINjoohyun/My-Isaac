#pragma once
#include "GameObject.h"

class TextBox : public GameObject
{
protected:
	Origins textsort;
	bool isTyping = false;
	
public:
	sf::RectangleShape box;

	std::string fontId;
	sf::Text text;

	TextBox(const std::string& fontId, const std::string& str, const std::string & n = "", Origins textsort = Origins::C);
	virtual ~TextBox() override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void Typing();
};