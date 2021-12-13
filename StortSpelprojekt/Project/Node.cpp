#include "Node.h"

Node::Node()
{
}

Node::~Node()
{	
	for (int i = 0; i < neighbours.size(); i++)
	{
		delete neighbours[i];
	}
}

Node::Node(Vector3 position, int id)
{
	this->position = position;
	this->id = id;
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

	return 0;
}

int Node::getFCost()
{
	return gCost + hCost;
}

void Node::Update()
{
}