#include "stdafx.h"
#include "MiniMap.h"
#include "SceneMgr.h"
#include "SceneGame.h"

void MiniMap::Init()
{
	SpriteGameObject::Init();

	SetOrigin(Origins::C);
	sprite.setTextureRect({ 0, 0, 55, 49 });
	sprite.setScale(3.5f, 3.5f);

	bossIcon.setTextureRect({ 34, 82, 9, 8 });
}
void MiniMap::Reset()
{
	SpriteGameObject::Reset();

	isOpenBoss = false;
	bossIcon.setTexture(*sprite.getTexture());
	RenewMiniMap();
}
void MiniMap::Draw(sf::RenderWindow& window)
{
	SpriteGameObject::Draw(window);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			window.draw(rooms[i][j]);
		}
	}
	if (isOpenBoss) window.draw(bossIcon);
}
void MiniMap::Release()
{
	SpriteGameObject::Release();
}

void MiniMap::SetPosition(const sf::Vector2f& position)
{
	SpriteGameObject::SetPosition(position);
}

void MiniMap::SetOrigin(Origins origin)
{
	SpriteGameObject::SetOrigin(origin);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Utils::SetOrigin(rooms[i][j], origin);
		}
	}
	Utils::SetOrigin(bossIcon, origin);
}

void MiniMap::RenewMiniMap()
{
	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			rooms[i][j].setTexture(*sprite.getTexture());

			rooms[i][j].setTextureRect({ 0, 0, 1, 1 });
			rooms[i][j].setPosition({ 40.0f + 17 * i, 40.0f + 15 * j });

			if (IsNear(i, j))
			{
				rooms[i][j].setTextureRect({ 0, 224, 17, 15 });
			}
			if (scene->stage1[i][j].isPassed)
			{
				rooms[i][j].setTextureRect({ 0, 160, 17, 15 });
			}
			if (scene->stage1[i][j].isHear)
			{
				rooms[i][j].setTextureRect({ 0, 192, 17, 15 });
			}
			if (scene->stage1[i][j].tag == 'B' && scene->stage1[i][j].isClarify)
			{
				bossIcon.setPosition(40.0f + 17 * i, 40.0f + 15 * j);
				isOpenBoss = true;
			}
		}
	}

	SetOrigin(origin);
}
const bool MiniMap::IsNear(int i, int j) const
{
	SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrentScene();

	if (scene->stage1[i][j].tag == NULL) return false;

	if (scene->stage1[i][j - 1].isHear && j > 0)
	{
		scene->stage1[i][j].isClarify = true;
	}
	if (scene->stage1[i][j + 1].isHear && j < 8)
	{
		scene->stage1[i][j].isClarify = true;
	}
	if (scene->stage1[i - 1][j].isHear && i > 0)
	{
		scene->stage1[i][j].isClarify = true;
	}
	if (scene->stage1[i + 1][j].isHear && i < 8)
	{
		scene->stage1[i][j].isClarify = true;
	}
	return scene->stage1[i][j].isClarify;
}