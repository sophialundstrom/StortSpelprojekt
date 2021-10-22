#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
	//grid.CreateGrid();
}

void Pathfinding::Update()
{
}

void Pathfinding::FindPath(Vector3 startPos, Vector3 TargetPos)
{
	Node startNode = grid.NodeFromWorldPoint(startPos);
	Node targetNode = grid.NodeFromWorldPoint(TargetPos);

	std::vector<Node> openSet;
	std::unordered_set<Node, MyHash> closedSet;

	openSet.push_back(startNode);

	// main loop for the path algorithm
	while (openSet.size() > 0)
	{
		Node node = openSet[0];
		for (int i = 0; i < openSet.size(); i++)
		{
			if (openSet[i].getFCost() < node.getFCost() || openSet[i].getFCost() == node.getFCost()) {
				if (openSet[i].hCost < node.hCost)
					node = openSet[i];
			}
		}
	}
}
