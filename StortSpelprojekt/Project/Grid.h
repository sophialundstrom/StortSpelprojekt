#pragma once
#include "Node.h"
#include "Transform.h"
#include "Drawable.h"
#include <map>
#include <cmath>
#include <algorithm>


class Grid : public Transform
{
private:
	int gridSizeX, gridSizeY;

	Vector2 gridWorldSize;
	float nodeRadius;
	Node grid[32][32]; // placeholder numbers
	float nodeDiameter;

public:
	Grid();

	void CreateGrid(std::map<std::string, std::shared_ptr<Drawable>> &drawable);
	Node NodeFromWorldPoint(Vector3 worldPoint);
};