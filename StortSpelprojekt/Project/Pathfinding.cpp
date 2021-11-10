#include "Pathfinding.h"


Pathfinding::Pathfinding()
{
	//grid.CreateGrid();
}

void Pathfinding::Update()
{
}

void Pathfinding::CreateGrid(/*std::map<std::string, std::shared_ptr<Drawable>>& drawable,*/ Vector3 worldPoint)
{
	grid.CreateGrid(colliders, worldPoint, this->heightMap);
}

void Pathfinding::FindPath(Vector3 startPos, Vector3 TargetPos)
{
	Node *startNode = grid.NodeFromWorldPoint(startPos);
	Node *targetNode = grid.NodeFromWorldPoint(TargetPos);
	startNode->walkable = true;
	targetNode->walkable = true;

	std::vector<Node*> openSet;
	std::unordered_set<Node*> closedSet;

	openSet.push_back(startNode); // add to the end of openset
	int index = 0;
	// main loop for the path algorithm
	while (openSet.size() > 0)
	{
		Node *node = openSet[0];
		for (int i = 0; i < openSet.size(); i++)
		{
			if (openSet[i]->getFCost() < node->getFCost() || openSet[i]->getFCost() == node->getFCost()) {
				if (openSet[i]->hCost < node->hCost)
				{
					node = openSet[i];
					index = i;
				}
			}
		}
		std::vector<Node*>::iterator nth = openSet.begin() + index;

		openSet.erase(nth); // remove the last element from the open set
		closedSet.insert(node);

		if (node == targetNode)
		{
			//retrace path. fills up a path vector in Grid
			grid.RetracePath(startNode, targetNode);
			for (int x = 0; x < 128; x++)
			{
				for (int y = 0; y < 128; y++)
				{
					grid.grid[x][y].gCost = 0;
					grid.grid[x][y].hCost = 0;
				}
			}
			return;
		}

		for (auto neighbour : grid.GetNeighbours(node))
		{
			// check if the neighbour is in the closed set or is walkable
			// would like to use .contains for the set but that is c++20 code
			if (!neighbour->walkable || closedSet.find(neighbour) != closedSet.end()) //!(std::none_of(closedSet.begin(), closedSet.end(), neighbour))) {
			{
				continue; 
			}

			int newCostToNeighbour = node->gCost + GetDistance(node, neighbour);
			if (newCostToNeighbour < neighbour->gCost || !(std::find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))
			{
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, targetNode);
				neighbour->parent = node;

				//if openset contains "neighbour" node
				if (!(std::find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))//	!std::any_of(openSet.begin(), openSet.end(), (Node)*neighbour))
					openSet.push_back(neighbour);
				else
				{
					//std::swap()
					std::swap(openSet[0], openSet[openSet.size()]);
				}
			}
			//grid.SetGridNode(neighbour->gridX, neighbour->gridY, *neighbour);
		}
	}
}
