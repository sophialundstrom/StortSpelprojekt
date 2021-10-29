#pragma once
#include "Math.h"
#include "Print.h"
#include "Collision.h"

class Node
{
private:

public:
	BoundingSphere BSphere;
	bool walkable = true;
	Vector3 position;
	int heapIndex;

	int gridX, gridY, gCost = 0, hCost = 0;
	Node* parent = nullptr;

	int fCost = 0;

	Node();
	virtual ~Node();
	Node(bool walkable, Vector3 position, int gridX, int gridY);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost, Node* parent);

	bool operator == (const Node& n) const
	{
		return (position == n.position /*&& parent == n.parent*/);
	}
	bool operator != (const Node& n) const
	{
		return (gridX != n.gridX && gridY != n.gridY /*&& parent == n.parent*/);
	}
	bool operator()(Node const& n)
	{
		bool cond = false;
		if (this->gridX == n.gridX && this->gridY == n.gridY)
			cond = true;
		return cond;
	}
	Node& operator= (Node&& n) noexcept { return *this; }

	int getFCost();
	int Compare(const Node* n);
	void Update();
	int CompareF(Node* node);
};

struct MyHash
{
	std::size_t operator()(Node const* n) const noexcept
	{
		std::size_t h1 = std::hash<int>{}(n->gridX);
		std::size_t h2 = std::hash<int>{}(n->gridY);
		return h1 ^ (h2 << 1); // or use boost::hash_combine
	}
};

inline float GetDistance(Node* n1, Node* n2)
{
	int dstX = abs(n1->gridX - n2->gridX);
	int dstY = abs(n1->gridY - n2->gridY);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
};
//namespace std
//{
//	template<> struct hash<Node>
//	{
//		std::size_t operator()(Node const& n) const noexcept
//		{
//			std::size_t h1 = std::hash<int>{}(n.gridX);
//			std::size_t h2 = std::hash<int>{}(n.gridY);
//			return h1 ^ (h2 << 1); // or use boost::hash_combine
//		}
//	};
//}