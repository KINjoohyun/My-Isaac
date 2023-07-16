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
};

enum class ObjType
{
	None = -1,

};

#include "AnimationClip.h"