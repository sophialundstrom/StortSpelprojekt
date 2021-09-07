#pragma once
#include "Math.h"
#include <vector>

struct Joint
{
	std::string name;
	Matrix offsetMatrix;
};

struct Skeleton
{
	std::vector<Matrix> transforms;
	std::vector<Joint> joints;
};