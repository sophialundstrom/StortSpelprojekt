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
	startNode->gCost = 0;
	Node *targetNode = grid.NodeFromWorldPoint(TargetPos);

	std::vector<Node*> openSet;
	std::unordered_set<Node*> closedSet;

	openSet.push_back(startNode); // add to the end of openset
	int index = 0;
	// main loop for the path algorithm
	while (openSet.size() > 0)
	{
		Node *node = openSet[0];
		index = 0;
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
			for (auto [i,n] : grid.GetNodes())
			{
				n->gCost = BIG_INT;
			}
			return;
		}

		for (auto [id,neighbour] : grid.GetNeighbours(node))
		{
			//if (neighbour->walkable == false)
			//	continue;

			int newCostToNeighbour = node->gCost + GetDistance(node, neighbour);
			if (newCostToNeighbour < neighbour->gCost)
			{
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, targetNode);
				neighbour->parent = node;

				//if openset does not contains "neighbour" node
				if ((std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()))
					openSet.push_back(neighbour);
			}
		}
	}
}

Pathfinding& Pathfinding::GetInstance()
{
	static Pathfinding singleton;
	return singleton;
}

Pathfinding* Pathfinding::PGetInstance()
{
	static Pathfinding* singleton;
	return singleton;
}



Node* Pathfinding::GetClosestNode(Vector3 worldPoint, int cutoff)
{
	return grid.NodeFromWorldPoint(worldPoint, cutoff);
}
