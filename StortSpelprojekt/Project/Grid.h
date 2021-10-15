#pragma once
#include "Node.h"
#include "Transform.h"
#include "Drawable.h"
#include <map>



class Grid : public Transform
{
private:
	Vector2 gridWorldSize;
	float nodeRadius;
	Node *grid; // placeholder numbers
	float nodeDiameter;
	int gridSizeX, gridSizeY;

public:
	Grid();

	void CreateGrid(std::map<std::string, std::shared_ptr<Drawable>> &drawable);

};