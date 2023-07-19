#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class SpriteGameObject;
class Poop;

class SceneGame : public Scene
{
protected:

	Player* player;
	std::vector<GameObject*> lifebar;

	std::list<Poop*> poops;

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

	void RenewLife(int life);
	void ViewSet(const sf::Vector2f& position);
	SpriteGameObject* LoadObj(ObjType objtype, const std::string& textureId);
	const std::list<Poop*>* GetPoopList() const;

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