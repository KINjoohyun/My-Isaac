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
class Monster;
class TextGameObject;
class MiniMap;
class GuideObject;

struct Room
{
	char tag = NULL;
	sf::Vector2f pos;
	sf::FloatRect wall;
	std::list<Monster*> monsters;
	std::vector<Door*> doors;

	bool isHear = false;
	bool isPassed = false;
	bool isClarify = false;
};

class SceneGame : public Scene
{
protected:

	Player* player;
	std::vector<GameObject*> lifebar;

	std::vector<GameObject*> objlist;
	std::list<RoomObject*> hitablelist;

	ObjectPool<Blood> poolBloods;

	float fps = 0.0f;
	float fpstimer = 0.0f;
	TextGameObject* fpstext;
	GuideObject* guide;
	SpriteGameObject* pauseObject;

	bool isAlive = true;
	bool isPause = false;

	float timer = 0.0f;
	std::tuple<int, int> bossroom = {0, 0};
	bool stage1clear = false;

	sf::Sound bgm;

public:
	
	MiniMap* minimap;
	Room stage1[9][9];

	bool isDebug = false;

	SceneGame();
	virtual ~SceneGame() override;

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	void CallRoom(const std::string& roomPath, const sf::Vector2f& position, int r, int c);
	void SetDoor(int r, int c);
	void RandomRooms();
	void ClearRooms();

	void RenewLife(int life);
	void ViewSet(const sf::Vector2f& position);
	SpriteGameObject* LoadObj(ObjType objtype, const std::string& textureId, const sf::FloatRect& wall, int r, int c);
	const std::list<RoomObject*>* GetPoopList() const;
	void RemoveRoomGO(RoomObject* roomGO);
	void RemoveMonster(Monster* monster, int r, int c);
	void DoorControl(int r, int c);
	void OnDiePlayer();
	void PrintMenu();
	void PrintGuide(const std::string& text);
	void PrintGameclear();

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