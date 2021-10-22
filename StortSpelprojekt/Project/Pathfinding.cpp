#include "Pathfinding.h"

Pathfinding::Pathfinding()
{
	//grid.CreateGrid();
}

void Pathfinding::Update()
{
}

void Pathfinding::CreateGrid(std::map<std::string, std::shared_ptr<Drawable>>& drawable)
{
	grid.CreateGrid(drawable);
}

void Pathfinding::FindPath(Vector3 startPos, Vector3 TargetPos)
{
	Node startNode = grid.NodeFromWorldPoint(startPos);
	Node targetNode = grid.NodeFromWorldPoint(TargetPos);

	std::vector<Node> openSet;
	std::unordered_set<Node, MyHash> closedSet;

	openSet.push_back(startNode); // add to the end of openset

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

		openSet.pop_back(); // remove the last element from the open set
		closedSet.insert(node);

		if (node == targetNode)
		{
			//retrace path. fills up a path vector in Grid
			grid.RetracePath(startNode, targetNode);
			return;
		}

		for (Node neighbour : grid.GetNeighbours(node))
		{
			// check if the neighbour is in the closed set or is walkable
			// would like to use .contains for the set but that is c++20 code
			if (!neighbour.walkable || (std::none_of(closedSet.begin(), closedSet.end(), (neighbour)))) {
				continue;
			}

			int newCostToNeighbour = node.gCost + GetDistance(node, neighbour);
			if (newCostToNeighbour < neighbour.gCost || std::none_of(openSet.begin(), openSet.end(), (neighbour))) 
			{
				neighbour.gCost = newCostToNeighbour;
				neighbour.hCost = GetDistance(neighbour, targetNode);
				neighbour.parent = &node;

				//if openset contains "neighbour" node
				if (std::none_of(openSet.begin(), openSet.end(), (neighbour)))
					openSet.emplace_back(neighbour);
			}
		}
	}
}
