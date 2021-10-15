#pragma once
#include "Math.h"
#include "Print.h"
#include "Collision.h"

class Node
{
private: 

public:
	BoundingSphere BSphere = { { 0,0,0}, {1.0f} };
	bool walkable;
	Vector3 position;

	Node();
	Node(bool walkable, Vector3 position);
	void Update();
};