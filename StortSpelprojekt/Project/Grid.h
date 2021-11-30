#pragma once
#include "Node.h"
#include "Drawable.h"
#include "Terrain.h"
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Collision.h"


class Grid : public Transform
{
private:
	const static int gridWorldSizeInt = 128;

	std::ifstream in;

	float nodeRadius;

	float nodeDiameter;

	std::map<int, Node*> m;
	std::vector<Vector3> waypointPath;
	//std::vector<Vector3> optimizePath(std::vector<Node*> path);
	//Node* grid[gridWorldSizeInt];

public:
	Grid();
	void CreateGrid(std::vector<std::shared_ptr<Collider>> colliders, Vector3 worldPosition, HeightMap* heightMap);
	// fetches node within 32 units of given position, otherwise return nullptr
	Node* NodeFromWorldPoint(Vector3 worldPoint);
	std::map<int, Node*> GetNeighbours(Node* node);
	void RetracePath(Node* startNode, Node* endNode);

	std::vector<Vector3> GetPath();
	std::vector<Vector3>& GetPathRef();
};