#include "Grid.h"
#include "Time.h"
//#include "Event.h"
#include <fstream>
#include "FileSystem.h"
#include <iostream>
const int size = 32;

void Grid::CreateGrid(std::vector<std::shared_ptr<Collider>> colliders, Vector3 worldPosition, HeightMap* heightMap)
{
	position = worldPosition;
	int nrUnwalkable = 0;
	Timer timer;
	auto filePath = FileSystem::ProjectDirectory::path;
	in.open(filePath + "\\SaveData\\Test.txt");
	std::string str;
	if (in.is_open())
	{
		while (std::getline(in, str))
		{





			Node* node = new Node();
		}
	}






	float time = timer.DeltaTime();
	std::fstream fs;
	fs.open(filePath + "\\SaveData\\Measurements.txt");
	fs.seekg(0, std::ios::end);
	fs.write(std::string(std::to_string(time)).c_str(), sizeof(std::string(std::to_string(time)).c_str()));
	fs.write(std::string("\n").c_str(), sizeof(std::string("\n").c_str()));
	fs.close();
	//exit(0);
}

void Grid::UpdateGrid(Vector3 worldPoint, std::vector<std::shared_ptr<Collider>>& colliders)
{
	Node* node = NodeFromWorldPoint(worldPoint);

	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			if (x == 0 && y == 0)
				continue;

			int checkX = node->gridX + x;
			int checkY = node->gridY + y;

			// check if node exist in existing grid
			if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY) {
			
				for (auto& collider : colliders)
				{
					// reduce the amount colliders
					if (Vector3::Distance(worldPoint, collider->GetPosition()) < 50.0f)
					{
						auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
						if (box)
						{
							if (Collision::Intersection(grid[x][y].BSphere.GetBounds(), box->GetBounds()))
							{
								grid[x][y].walkable = false;
							}
							else
							{
								grid[x][y].walkable = false;
							}
							continue;
						}
						auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
						if (sphere)
						{
							if (Collision::Intersection(grid[x][y].BSphere.GetBounds(), sphere->GetBounds()))
							{
								grid[x][y].walkable = false;
							}
							else
							{
								grid[x][y].walkable = false;
							}
							continue;
						}
					}
				}
			}
		}
	}
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
	if (!path.size() < 3)
		waypoints.push_back(path[path.size() - 2]->position);
	std::reverse(waypoints.begin(), waypoints.end());
	this->waypointPath = waypoints;
}

std::vector<Vector3> Grid::GetPath()
{
	return waypointPath;
}
std::vector<Vector3>& Grid::GetPathRef()
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
