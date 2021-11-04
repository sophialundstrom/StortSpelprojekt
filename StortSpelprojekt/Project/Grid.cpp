#include "Grid.h"
const int size = 32;

void Grid::CreateGrid(std::map<std::string, std::shared_ptr<Drawable>> &drawable)
{
	Vector3 worldBottomLeft = position - (Vector3::Right * gridWorldSize.x / 2) - (Vector3::Forward * gridWorldSize.y / 2); // check values of up and right

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			Vector3 worldPoint = worldBottomLeft + Vector3::Right * (x * nodeDiameter + nodeRadius) + /*Vector3(0,0,1)*/Vector3::Forward * (y * nodeDiameter + nodeRadius);

			grid[x][y].position = worldPoint;
			grid[x][y].gridX = x;
			grid[x][y].gridY = y;

			for (auto& [name, drawable] : drawable)
			{
				if (Vector3::Distance(worldPoint, drawable->GetPosition()) < 4.0f)
				{
					BoundingSphere tmpSphere = { {Matrix::Identity}, {drawable->GetPosition()}, {1.0f} }; // tempporary solution to a colliding issue
					if (grid[x][y].BSphere.GetBounds().Intersects(tmpSphere.GetBounds()))
					{
						if (name != "RainingGATOS")
						{
							grid[x][y].walkable = false; // this does happen sometimes 

						}
					}
				}
			}
		}
	}
	Print("grid completed");
}

Node* Grid::NodeFromWorldPoint(Vector3 worldPoint)
{
	float percentX = (worldPoint.x + gridWorldSize.x / 2) / gridWorldSize.x;
	float percentY = (worldPoint.z + gridWorldSize.y / 2) / gridWorldSize.y;

	std::clamp(percentX, 0.0f, 1.0f);
	std::clamp(percentY, 0.0f, 1.0f);


	int x = rint((gridSizeX - 1) * percentX);
	int y = rint((gridSizeY - 1) * percentY);
	return &grid[x][y];
}

std::vector<Node*> Grid::GetNeighbours(Node *node)
{
	std::vector<Node*> neighbours;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0)
				continue;

			int checkX = node->gridX + x;
			int checkY = node->gridY + y;

			if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY) {
				neighbours.push_back(&grid[checkX][checkY]);
			}
		}
	}

	return neighbours;
}

void Grid::RetracePath(Node *startNode, Node *endNode)
{
	std::vector<Node*> path;
	Node *currentNode = endNode;

	while (currentNode != startNode)
	{
		path.emplace_back(currentNode);
		currentNode = currentNode->parent;
	}
	std::reverse(path.begin(), path.end());
	this->path = path;
}

std::vector<Node*> Grid::GetPath()
{
	return path;
}

Grid::Grid()
	:Transform(Vector3::Zero, Vector3::Zero, Vector3::Zero)
{
	for (int i = 0; i < 32; i++)
	{
		grid[i] = new Node[32];
	}
	nodeRadius = 0.5f;
	gridWorldSize = Vector2(32,32);
	nodeDiameter = nodeRadius * 2;
	gridSizeX = roundToInt(gridWorldSize.x/ nodeDiameter);
	gridSizeY = roundToInt(gridWorldSize.y / nodeDiameter);
}
