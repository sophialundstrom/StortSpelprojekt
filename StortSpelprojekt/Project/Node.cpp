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

int Node::CompareTo(Node *n)
{
	int compare = Compare(this->getFCost(), n->getFCost());
	if (compare == 0)
	{
		compare = Compare(this->hCost, n->hCost);
	}
	return -compare; // out nodes are reversed
}

int Node::getFCost()
{
	return gCost + hCost;
}

void Node::Update()
{
}
