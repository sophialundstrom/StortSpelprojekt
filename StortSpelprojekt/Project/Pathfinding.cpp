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
	Node *startNode = grid.NodeFromWorldPoint(startPos);
	Node *targetNode = grid.NodeFromWorldPoint(TargetPos);

	std::unordered_set<Node*> closedSet;

	openSet.push(startNode); // add to the end of openset

	// main loop for the path algorithm
	while (openSet.size() > 0)
	{
		Node* node = openSet.top();// ->RemoveFirst();// remove the last element from the open set
		node->inOpenSet = false;

		openSet.pop();

		closedSet.insert(node);

		if (node == targetNode)
		{
			//retrace path. fills up a path vector in Grid
			grid.RetracePath(startNode, targetNode);
			return;
		}

		for (auto neighbour : grid.GetNeighbours(node))
		{
			// check if the neighbour is in the closed set or is walkable
			if (!neighbour->walkable || closedSet.find(neighbour) != closedSet.end()) //!(std::none_of(closedSet.begin(), closedSet.end(), neighbour))) {
			{
				continue;
			}

			int newCostToNeighbour = node->gCost + GetDistance(node, neighbour);
			if (newCostToNeighbour < neighbour->gCost || !neighbour->inOpenSet)
			{
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, targetNode);
				neighbour->parent = node;

				//if openset does not contains "neighbour" node
				if (!neighbour->inOpenSet)//	!std::any_of(openSet.begin(), openSet.end(), (Node)*neighbour))
				{
					openSet.push(neighbour);
					neighbour->inOpenSet = true;
				}
			}
			//grid.SetGridNode(neighbour->gridX, neighbour->gridY, *neighbour);
		}
	}
}
