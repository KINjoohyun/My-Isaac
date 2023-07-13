#pragma once

#define _USE_MATH_DEFINES

enum class Languages
{
	KOR,
	ENG,
	JAP,
	Count,
};

enum class SceneId
{
	None = -1,
	Title,
	Game,
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