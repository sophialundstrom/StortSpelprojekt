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

	int gridX, gridY, gCost = 0, hCost = 0;
	Node* parent = NULL;

	int fCost = 0;

	Node();
	~Node();
	Node(bool walkable, Vector3 position, int gridX, int gridY);
	Node(Node&& n) noexcept {};
	Node(const Node& n)
		:Node(n.walkable, n.position, n.gridX, n.gridY)
	{
	////BSphere = n.BSphere;
	//	fCost = n.fCost;
	//	gCost = n.gCost;
	//	gridX = n.gridX;
	//	gridY = n.gridY;
	//	hCost = n.hCost;
	//	parent = n.parent;
	//	position = n.position;
	//	walkable = n.walkable;
	};
	Node& operator= (Node n)// copy constructor
	{
		fCost = n.fCost;
		gCost = n.gCost;
		gridX = n.gridX;
		gridY = n.gridY;
		hCost = n.hCost;
		parent = n.parent;
		position = n.position;
		walkable = n.walkable;
		return *this;
	};
	bool operator == (const Node& n) const
	{
		return (gridX == n.gridX && gridY == n.gridY /*&& parent == n.parent*/);
	}
	Node& operator= (Node&& n) noexcept { return *this; }
	//Node& operator= (Node&& n) noexcept { return *this; }

	//std::hash<Node> h;

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