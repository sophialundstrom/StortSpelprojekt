#include "Node.h"

Node::Node()
{
}

Node::~Node()
{
	delete parent;
}

Node::Node(bool walkable, Vector3 position, int gridX, int gridY)
{
	this->walkable = walkable;
	this->position = position;
	this->BSphere.SetPosition(position);
	this->gridX = gridX;
	this->gridY = gridY;
}

int Node::getFCost()
{
	return gCost + hCost;
}

void Node::Update()
{
}
