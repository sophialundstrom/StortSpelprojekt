#pragma once
#include "Grid.h"
#include <unordered_set>
#include <vector>

class Pathfinding
{
private:
	Vector3 seeker, target;
	Grid grid;
	std::map<std::string, std::shared_ptr<Drawable>> drawables;
	std::vector<std::shared_ptr<Collider>> colliders;
	HeightMap* heightMap;
public:
	Pathfinding();

	void Update();
	void CreateGrid(/*std::map<std::string, std::shared_ptr<Drawable>>& drawable,*/ Vector3 worldPoint);
	void FindPath(Vector3 startPos, Vector3 TargetPos);
	Grid* GetGrid() { return &grid; }
	void SetDrawables(std::map<std::string, std::shared_ptr<Drawable>> d) { drawables = d; }
	void SetColliders(std::vector<std::shared_ptr<Collider>> c) { colliders = c; }
	void SetHeightMap(HeightMap* heightMap) { this->heightMap = heightMap; }
};