#pragma once
#include "Math.h"
#include "Print.h"
#include "Collision.h"
#include <limits>
#include <map>

#define BIG_INT 200000;

class Node
{
private:

public:
	Vector3 position;
	int gCost = BIG_INT;
	int hCost = 0;
	std::map<int, Node*> neighbours;
	Node* parent;

	Node();
	virtual ~Node();
	Node(Vector3 position);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost, Node* parent);

	bool operator == (const Node& n) const
	{
		return (position == n.position /*&& parent == n.parent*/);
	}
	bool operator != (const Node& n) const
	{
		return (position != n.position);
	}
	bool operator()(Node const& n)
	{
		bool cond = false;
		if (position == n.position)
			cond = true;
		return cond;
	}
	Node& operator= (Node&& n) noexcept { return *this; }

	int getFCost();
	int Compare(const Node* n);
	void Update();
	int CompareF(Node* node);
};

inline float GetDistance(Node* n1, Node* n2)
{
	return Vector3::Distance(n1->position, n2->position);
	//int dstX = abs(n1->gridX - n2->gridX);
	//int dstY = abs(n1->gridY - n2->gridY);

	//if (dstX > dstY)
	//	return 14 * dstY + 10 * (dstX - dstY);
	//return 14 * dstX + 10 * (dstY - dstX);
};
