#pragma once
#include "Math.h"
#include "Print.h"
#include "Collision.h"

class Node
{
private:

public:
	BoundingSphere BSphere = { { 0,0,0}, {1.0f} };
	bool walkable = true;
	Vector3 position;

	int gridX, gridY, gCost = 0, hCost = 0;
	Node* parent = NULL;

	int fCost = 0;

	Node();
	~Node();
	Node(bool walkable, Vector3 position, int gridX, int gridY);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost);
	Node(bool walkable, Vector3 position, int gridX, int gridY, int hCost, int gCost, Node* parent);

	template<typename _Ty>
	Node(_Ty)  {}

	Node(const Node& n)
		:Node(n.walkable, n.position, n.gridX, n.gridY, n.hCost, n.gCost, n.parent)
	{}
	Node& operator= ( Node n)// copy assignment
	{
		std::swap(gridX, n.gridX);
		std::swap(gridY, n.gridY);
		std::swap(gCost, n.gCost);
		std::swap(hCost, n.hCost);
		std::swap(fCost, n.fCost);
		std::swap(parent, n.parent);
		std::swap(walkable, n.walkable);
		std::swap(position, n.position);
		//fCost = n.fCost;
		//gCost = n.gCost;
		//gridX = n.gridX;
		//gridY = n.gridY;
		//hCost = n.hCost;
		//parent = n.parent;
		//position = n.position;
		//walkable = n.walkable;
		return *this;
	}
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
	void Update();
};

struct MyHash
{
	std::size_t operator()(Node const& n) const noexcept
	{
		std::size_t h1 = std::hash<int>{}(n.gridX);
		std::size_t h2 = std::hash<int>{}(n.gridY);
		return h1 ^ (h2 << 1); // or use boost::hash_combine
	}
};

inline float GetDistance(Node n1, Node n2)
{
	int dstX = abs(n1.gridX - n2.gridX);
	int dstY = abs(n1.gridY - n2.gridY);

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