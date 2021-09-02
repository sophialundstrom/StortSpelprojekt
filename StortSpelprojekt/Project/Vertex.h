#pragma once
#include "Math.h"

//VERTEX
struct TempVertex
{
	Vector3 position;
	Vector2 texCoords;
	Vector3 normal;
	Vector4 weights;
	UINT boneIDs[4];
};
