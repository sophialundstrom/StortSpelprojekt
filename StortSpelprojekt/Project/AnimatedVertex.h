#pragma once
#include "Math.h"

#define UNDEF -1

struct AnimatedVertex
{
	Vector3 position;
	Vector2 texCoords;
	Vector3 normal;
	FLOAT weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	INT boneIDs[4] = { UNDEF, UNDEF, UNDEF, UNDEF };
};