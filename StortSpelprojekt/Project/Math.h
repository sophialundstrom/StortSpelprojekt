#pragma once
#include "DirectXTK\SimpleMath.h"

using namespace DirectX::SimpleMath;

constexpr float PI = 3.14159265359f;
constexpr float PI_DIV2 = PI / 2.0f;
constexpr float PI_DIV4 = PI / 4.0f;

inline int roundToInt(float number)
{
	int returnValue;
	if (number - (int)number < 0.5f)
	{
		returnValue = (int)number;
	}
	else if (number - (int)number >= 0.5f)
	{
		returnValue = (int)number + 1;
	}
	return returnValue;
};