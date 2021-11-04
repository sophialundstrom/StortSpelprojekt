#pragma once
#include "Heap.h"
#include <unordered_set>
#include <vector>
#include <queue>

class Pathfinding
{
private:
	Vector3 seeker, target;
	Grid grid;
	//Heap<Node>* openSet = new Heap<Node>(32 * 32);
	std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> openSet;

public:
	Pathfinding();

	void Update();
	void CreateGrid(std::map<std::string, std::shared_ptr<Drawable>>& drawable);
	void FindPath(Vector3 startPos, Vector3 TargetPos);
	
};