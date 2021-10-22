#pragma once
#include "Grid.h"
#include <unordered_set>
#include <vector>

class Pathfinding
{
private:
	Vector3 seeker, target;
	Grid grid;

public:
	Pathfinding();
	void Update();
	void FindPath(Vector3 startPos, Vector3 TargetPos);
};