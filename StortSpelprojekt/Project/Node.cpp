#include "Node.h"

Node::Node()
{
}

Node::~Node()
{
	//delete parent;
}

Node::Node(bool walkable, Vector3 position, int gridX, int gridY)
{
	this->walkable = walkable;
	this->position = position;
	this->BSphere.SetPosition(position);
	this->gridX = gridX;
	this->gridY = gridY;
}

Node::Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost)
{
	this->walkable = walkable;
	this->position = position;
	this->BSphere.SetPosition(position);
	this->gridX = gridX;
	this->gridY = gridY;
	this->hCost = hCost;
	this->gCost = gCost;
}

Node::Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost, Node* parent)
{
	this->walkable = walkable;
	this->position = position;
	this->BSphere.SetPosition(position);
	this->gridX = gridX;
	this->gridY = gridY;
	this->hCost = hCost;
	this->gCost = gCost;
	this->parent = parent;
}

int Node::Compare(const Node* n)
{
	if (this->getFCost() > n->gCost + n->hCost)
	{
		return 1;
	}
	else if(this->getFCost() == n->gCost + n->hCost)
	{
		return 0;
	}
	else if (this->getFCost() < n->gCost + n->hCost)
	{
		return -1;
	}
}

int Node::getFCost()
{
	return gCost + hCost;
}

void Node::Update()
{
}

Vector2 Node::GetGridCoord()
{
	return Vector2(this->gridX, this->gridY);
}
