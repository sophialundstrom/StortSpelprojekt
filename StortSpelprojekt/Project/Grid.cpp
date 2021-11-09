#include "Grid.h"
const int size = 32;

void Grid::CreateGrid(std::map<std::string, std::shared_ptr<Drawable>>& drawable, Vector3 worldPosition, HeightMap* heightMap)
{
	position = worldPosition;
	Vector3 worldBottomLeft = position - (Vector3::Right * gridWorldSize.x / 2) - (Vector3::Forward * gridWorldSize.y / 2);
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			Vector3 worldPoint = worldBottomLeft + Vector3::Right * (x * nodeDiameter + nodeRadius) + /*Vector3(0,0,1)*/Vector3::Forward * (y * nodeDiameter + nodeRadius);

			worldPoint.y = heightMap->data.at(Vector2((int)worldPoint.x, (int)worldPoint.z));
			grid[x][y].position = worldPoint;
			grid[x][y].gridX = x;
			grid[x][y].gridY = y;

			for (auto& [name, drawable] : drawable)
			{
				if (Vector3::Distance(worldPoint, drawable->GetPosition()) < 4.0f)
				{
					BoundingSphere tmpSphere = { {Matrix::Identity}, {drawable->GetPosition()}, {2.0f} }; // tempporary solution to a colliding issue
					if (grid[x][y].BSphere.GetBounds().Intersects(tmpSphere.GetBounds()))
					{
						grid[x][y].walkable = false;
					}
				}
			}
		}
	}
	Print("grid completed");
}

Node* Grid::NodeFromWorldPoint(Vector3 worldPoint)
{
	// move the grid -- -position.axis -- to get it to world zero
	float percentX = ((worldPoint.x - position.x) + gridWorldSize.x / 2) / gridWorldSize.x;
	float percentY = ((worldPoint.z - position.z) + gridWorldSize.y / 2) / gridWorldSize.y;

	percentX = std::clamp(percentX, 0.0f, 1.0f);
	percentY = std::clamp(percentY, 0.0f, 1.0f);


	int x = rint((gridSizeX - 1) * percentX);
	int y = rint((gridSizeY - 1) * percentY);
	//std::cout << grid[x][y].position.x << ", "<< grid[x][y].position.y << ", " << grid[x][y].position.z << std::endl;
	//std::cout << grid[x][31 - y].position.x << ", " << grid[x][31 - y].position.y << ", " << grid[x][31 - y].position.z << std::endl;
	//std::cout << grid[31 - x][31 - y].position.x << ", " << grid[31 - x][31 - y].position.y << ", " << grid[31 - x][31 - y].position.z << std::endl;
	return &grid[x][gridWorldSizeInt-1 - y];
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
		path.push_back(currentNode);
		currentNode = currentNode->parent;
	}
	std::vector<Vector3> waypoints = optimizePath(path);
	if (!path.size() < 1)
		waypoints.push_back(path[path.size() - 1]->position);
	std::reverse(waypoints.begin(), waypoints.end());
	this->waypointPath = waypoints;
}

std::vector<Vector3> Grid::GetPath()
{
	return waypointPath;
}

std::vector<Vector3> Grid::optimizePath(std::vector<Node*> path)
{
	std::vector<Vector3> waypoints;
	Vector2 directionOld = Vector2::Zero;

	for (int i = 1; i < path.size(); i++)
	{
		// may have to change this to world coordinates instead of indexes. but it might work
		Vector2 directionNew = Vector2(path[i - (int)1]->gridX - path[i]->gridX, path[i - (int)1]->gridY - path[i]->gridY);
		if (directionNew != directionOld)
		{
			waypoints.push_back(path[i]->position);
		}
		directionOld = directionNew;
	}


	return waypoints;
}

Grid::Grid()
	:Transform(Vector3::Zero, Vector3::Zero, Vector3::Zero)
{
	for (int i = 0; i < gridWorldSizeInt; i++)
	{
		grid[i] = new Node[gridWorldSizeInt];
	}
	nodeRadius = 0.5f;
	gridWorldSize = Vector2(gridWorldSizeInt, gridWorldSizeInt);
	nodeDiameter = nodeRadius * 2;
	gridSizeX = roundToInt(gridWorldSize.x/ nodeDiameter);
	gridSizeY = roundToInt(gridWorldSize.y / nodeDiameter);
}
