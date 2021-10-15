#include "Node.h"

Node::Node()
{
}

Node::Node(bool walkable, Vector3 position)
{
	this->walkable = walkable;
	this->position = position;
	this->BSphere.SetPosition(position);
}

void Node::Update()
{
}
