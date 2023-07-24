#pragma once
class AnimationController
{
protected:
	//std::unordered_map<std::string, AnimationClip> clips;
	std::unordered_map<std::string, AnimationClip*> clips;
	std::queue<std::string> queue;

	float speed = 1.0f;
	bool isPlaying = false;

	AnimationClip* currentClip = nullptr;
	int currentFrame = -1;
	int totalFrame = 0;
	float clipDuration = 0.0f;
	float accumTime = 0.0f;

	sf::Sprite* target = nullptr;

public:
	//void AddClip(const AnimationClip& newClip);
	void AddClip(AnimationClip& newClip);
	void SetTarget(sf::Sprite* sprite) { target = sprite; }
	sf::Sprite* GetTarget() const { return target; }
	bool IsPlaying() { return isPlaying; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetFrame(const AnimationFrame& frame);

	void Update(float dt);

	void Play(const std::string& clipId, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);
	void Stop();
	std::string GetCurrentClipId() const;
};