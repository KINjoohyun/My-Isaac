#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceMgr.h"

//void AnimationController::AddClip(const AnimationClip& newClip)
void AnimationController::AddClip(AnimationClip& newClip)
{
	if (clips.find(newClip.id) == clips.end())
	{
		//clips.insert({ newClip.id, newClip });
		clips.insert({ newClip.id, &newClip });
	}
}
void AnimationController::SetFrame(const AnimationFrame& frame)
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);

	target->setTexture(*tex);
	target->setTextureRect(frame.texCoord);
}

void AnimationController::Update(float dt)
{
	if (!isPlaying) return;

	accumTime += dt * speed;
	if (accumTime < clipDuration) return;

	accumTime = 0.0f;

	currentFrame++;
	if (currentFrame == totalFrame)
	{
		if (!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();
			Play(id, false);
			return;
		}
		else
		{
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				return;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			}
		}
	}

	if (currentClip->frames[currentFrame].action != nullptr)
	{
		currentClip->frames[currentFrame].action();
	}

	if (target != nullptr)
	{
		SetFrame(currentClip->frames[currentFrame]);
	}
}

void AnimationController::Play(const std::string& clipId, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty()) queue.pop();
	}

	auto find = clips.find(clipId);
	if (find == clips.end())
	{
		Stop();
		std::cerr << "ERROR: AnimationController Play()" << std::endl;
		return;
	}

	isPlaying = true;
	//currentClip = &find->second;
	currentClip = find->second;
	currentFrame = 0;
	totalFrame = currentClip->frames.size();
	clipDuration = 1.0f / currentClip->fps;
	accumTime = 0.0f;

	SetFrame(currentClip->frames[currentFrame]);
}
void AnimationController::PlayQueue(const std::string& clipId)
{
	queue.push(clipId);
}

void AnimationController::Stop()
{
	isPlaying = false;
}

std::string AnimationController::GetCurrentClipId() const
{
	if (currentClip == nullptr)
	{
		std::cout << "WARNING: Not Exist currentClip" << std::endl;
		return "";
	}
	return currentClip->id;
}
AnimationClip* AnimationController::GetCurrentClip() const
{
	return currentClip;
}