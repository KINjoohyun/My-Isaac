#pragma once
#define ROOM_INTERVAL (1200.0f)

#include "Scene.h"
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class SpriteGameObject;
class RoomObject;
class Blood;
class Door;

struct Room
{
	char tag = NULL;
	sf::Vector2f pos;
	sf::FloatRect wall;
};

class SceneGame : public Scene
{
protected:

	Player* player;
	std::vector<GameObject*> lifebar;

	std::list<RoomObject*> hitablelist;

	ObjectPool<Blood> poolBloods;

	Room stage1[9][9];

public:
	SceneGame();
	virtual ~SceneGame() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	void CallRoom(const std::string& roomPath, const sf::Vector2f& position);
	void SetDoor(int r, int c);

	void RenewLife(int life);
	void ViewSet(const sf::Vector2f& position);
	SpriteGameObject* LoadObj(ObjType objtype, const std::string& textureId, const sf::FloatRect& wall);
	const std::list<RoomObject*>* GetPoopList() const;
	void RemoveRGO(RoomObject* roomGO);

	template <typename T>
	void ClearPool(ObjectPool<T>& pool);
};

template<typename T>
inline void SceneGame::ClearPool(ObjectPool<T>& pool)
{
	for (auto it : pool.GetUseList())
	{
		RemoveGO(it);
	}
	pool.AllReturn();
}