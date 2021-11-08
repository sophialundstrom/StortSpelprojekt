#pragma once
#include "Node.h"
#include "Transform.h"
#include "Drawable.h"
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>


class Grid : public Transform
{
private:
	int gridSizeX, gridSizeY;

	Vector2 gridWorldSize;
	float nodeRadius;
	Node* grid[32]; // placeholder numbers

	float nodeDiameter;

	std::vector<Vector3> waypointPath;
	std::vector<Vector3> optimizePath(std::vector<Node*> path);

public:
	Grid();
	void CreateGrid(std::map<std::string, std::shared_ptr<Drawable>> &drawable, Vector3 worldPosition);
	Node* NodeFromWorldPoint(Vector3 worldPoint);
	std::vector<Node*> GetNeighbours(Node* node);
	void RetracePath(Node* startNode, Node* endNode);

	std::vector<Vector3> GetPath();
};