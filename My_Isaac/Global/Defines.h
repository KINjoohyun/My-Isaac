#pragma once

#define _USE_MATH_DEFINES

enum class Languages
{
	KOR,
	ENG,
	Count,
};

enum class SceneId
{
	None = -1,
	Title,
	Game,
	Editor,
	Count,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,
};

enum class Origins
{
	TL, //Top Left
	TC,
	TR,
	L,
	C,
	R,
	BL,
	BC,
	BR,
	CUSTOM
};

#include "AnimationClip.h"