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
	void CreateGrid(std::map<std::string, std::shared_ptr<Drawable>>& drawable);
	void FindPath(Vector3 startPos, Vector3 TargetPos);
	Grid* GetGrid() { return &grid; }
	
};