#pragma once

class GameObject;

class Scene
{
protected:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	SceneId sceneId;

	std::string resourceListPath;

	std::list<GameObject*> gameObjects;
	std::list<GameObject*> removeGameObjects;

	sf::View worldView;
	sf::View uiView;

	sf::RenderWindow& window;
	sf::Vector2f windowSize;
public:
	Scene(SceneId id = SceneId::None);
	virtual ~Scene();

	virtual void Init() = 0;
	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);
	virtual void Release() {}

	virtual void Enter();
	virtual void Exit();

	GameObject* FindGO(const std::string& name);
	void FindGOs(std::list<GameObject*>& list, const std::string& name);
	bool Exist(GameObject* go);
	GameObject* AddGO(GameObject* go);
	void RemoveGO(GameObject* go);
	void SortGOs();

	sf::Vector2f ScreenToWorldPos(sf::Vector2f screenPos);
	sf::Vector2f ScreenToUiPos(sf::Vector2f screenPos);
	sf::Vector2f WolrdPosToScreen(sf::Vector2f worldPos);
	sf::Vector2f UiPosToScreen(sf::Vector2f uiPos);
};