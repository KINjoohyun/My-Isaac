#pragma once

#define _USE_MATH_DEFINES

enum class Languages
{
	None = -1,
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
	None = -1,
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,

	Count,
};

enum class Origins
{
	None = -1,
	TL, //Top Left
	TC,
	TR,
	L,
	C,
	R,
	BL,
	BC,
	BR,
	CUSTOM,

	Count,
};

enum class ObjType
{
	None = -1,
	Rock,
	Poop,
	Spike,
	AttackFly,
	Pooter,
	Sucker,

	Count,
};

#include "AnimationClip.h"