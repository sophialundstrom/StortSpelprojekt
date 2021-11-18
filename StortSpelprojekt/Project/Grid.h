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


	int gridSizeX, gridSizeY;
	Vector2 gridWorldSize; 

	float nodeRadius;

	float nodeDiameter;

	std::vector<Vector3> waypointPath;
	std::vector<Vector3> optimizePath(std::vector<Node*> path);

public:
	Grid();
	const static int gridWorldSizeInt = 128;
	Node* grid[gridWorldSizeInt]; // placeholder numbers
	void CreateGrid(std::vector<std::shared_ptr<Collider>> colliders, Vector3 worldPosition, HeightMap* heightMap);
	void SubCreateGrid(std::map<std::string, std::shared_ptr<Drawable>>& drawable, Vector3 worldPosition, int subDivision, HeightMap* heightMap);
	void UpdateGrid(Vector3 worldPoint, )
	Node* NodeFromWorldPoint(Vector3 worldPoint, std::vector<std::shared_ptr<Collider>>& colliders);
	std::vector<Node*> GetNeighbours(Node* node);
	void RetracePath(Node* startNode, Node* endNode);

	std::vector<Vector3> GetPath();
	std::vector<Vector3>& GetPathRef();
};