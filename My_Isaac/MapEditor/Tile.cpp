#include "stdafx.h"
#include "Tile.h"
#include "Collider.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"

Tile::Tile(const ObjType objtype, const std::string& textureId, const std::string& name)
	:SpriteGameObject(textureId, name)
{
	this->objtype = objtype;

	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color::Red);
	box.setOutlineThickness(1);
	order.setString(std::to_string(sortOrder));
	order.setCharacterSize(20);
	order.setFillColor(sf::Color::Red);
}
Tile::~Tile()
{

}

void Tile::Init()
{
	SpriteGameObject::Init();
}
void Tile::Reset()
{
	SpriteGameObject::Reset();

	box.setSize({ sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
	Utils::SetOrigin(box, origin);
	Utils::SetOrigin(order, origin);
	isHover = false;
}
void Tile::Update(float dt)
{
	SpriteGameObject::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousPos = SCENE_MGR.GetCurrentScene()->ScreenToWorldPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(worldMousPos);
	
	if (isHover && INPUT_MGR.GetMouseButton(sf::Mouse::Left))
	{
		SetPosition(worldMousPos);
	}
	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Right))
	{
		SceneEditor* scene = (SceneEditor*)SCENE_MGR.GetCurrentScene();
		scene->TileRemove(this);
	}

	if (isHover && INPUT_MGR.GetMouseButton(sf::Mouse::Left) && INPUT_MGR.GetKeyUp(sf::Keyboard::Add))
	{
		sortOrder++;
		order.setString(std::to_string(sortOrder));
	}
	if (isHover && INPUT_MGR.GetMouseButton(sf::Mouse::Left) && INPUT_MGR.GetKeyUp(sf::Keyboard::Dash))
	{
		sortOrder--;
		order.setString(std::to_string(sortOrder));
	}
}
void Tile::Draw(sf::RenderWindow& window)
{
	SpriteGameObject::Draw(window);
	window.draw(box);
	window.draw(order);
}

void Tile::SetPosition(const sf::Vector2f& position)
{
	SpriteGameObject::SetPosition(position);
	box.setPosition(position);
	order.setPosition(position);
}
void Tile::SetPosition(float x, float y)
{
	SpriteGameObject::SetPosition(x, y);
	box.setPosition(x, y);
	order.setPosition(x, y);
}

void Tile::SetOrigin(Origins origin)
{
	SpriteGameObject::SetOrigin(origin);
	Utils::SetOrigin(box, origin);
	Utils::SetOrigin(order, origin);
}
void Tile::SetOrigin(float x, float y)
{
	SpriteGameObject::SetOrigin(origin);
	box.setOrigin(x, y);
	order.setOrigin(x, y);
}