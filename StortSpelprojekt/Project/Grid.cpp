#include "Grid.h"
#include "Time.h"
//#include "Event.h"
#include <fstream>
#include "FileSystem.h"
#include <iostream>
const int size = 32;

Grid::Grid()
	:Transform(Vector3::Zero, Vector3::Zero, Vector3::Zero)
{
}

void Grid::CreateGrid(std::vector<std::shared_ptr<Collider>> colliders, Vector3 worldPosition, HeightMap* heightMap)
{
	position = worldPosition;
	int nrUnwalkable = 0;
	Timer timer;
	timer.Start();
	auto filePath = FileSystem::ProjectDirectory::path;
	in.open(filePath + "\\SaveData\\Nodes.txt");
	std::string str;
	std::string fill;
	std::string id;
	std::string tmp;
	if (in.is_open())
	{
		while (std::getline(in, str))
		{
			if (str[0] == 'N')
			{
				std::stringstream ss(str);

				std::string id, Sx, Sy, Sz;
				int iid;
				float fx, fy, fz;
				std::getline(ss, id, '1');
				std::getline(ss, id, '\t');

				std::getline(ss, Sx, '\t');
				std::getline(ss, Sy, '\t');
				std::getline(ss, Sz); 
				if (id.empty())
					iid = 0;
				else
					iid = std::stoi(id);
				fx = std::stof(Sx);
				fy = std::stof(Sy) + 1.0f;
				fz = std::stof(Sz);
				m.emplace(iid, new Node(Vector3(fx, fy, fz), iid));
			}
			else if (str[0] == 'E')
			{
				str.erase(0, 2); // erase E and tab
				std::stringstream ss(str);

				std::string id;

				//process the first node
				std::getline(ss, id, '1');	// remove the node name so that only ID is left.
				std::getline(ss, id, '\t'); // Register the ID.
				if (id.empty())				// If it is the original set ID to 0.
					id = "0";
				auto node1 = m.find(std::stoi(id)); // Find the corresponding node
				// process the neighbour
				std::getline(ss, id, '1');
				std::getline(ss, id); std::cout << "\"" << id << "\"";
				if (id.empty())
					id = "0";
				auto node2 = m.find(std::stoi(id));
				// add any niegbour to the two nodes
				node1->second->neighbours.emplace(node2->first,node2->second);
				node2->second->neighbours.emplace(node1->first, node1->second);
			}
		}
	}






	float time = timer.DeltaTime();
	std::fstream fs;
	fs.open(filePath + "\\SaveData\\Measurements.txt");
	fs.seekg(0, std::ios::end);
	fs.write(std::string(std::to_string(time)).c_str(), sizeof(std::string(std::to_string(time)).c_str()));
	fs.write(std::string("\n").c_str(), sizeof(std::string("\n").c_str()));
	fs.close();
	OutputDebugStringA("done");
	//exit(0);
}


std::map<int, Node*> Grid::GetNeighbours(Node* node)
{
	return node->neighbours;
}

void Grid::RetracePath(Node* startNode, Node* endNode)
{
	std::vector<Node*> path;
	Node* currentNode = endNode;

	while (currentNode != startNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->parent;
	}
	if (currentNode == startNode)
		path.push_back(currentNode);
	std::vector<Vector3> waypoints;
	//if (!path.size() < 3)
	//	path.push_back(path[path.size() - 2]->position);
	std::reverse(path.begin(), path.end());
	ids.clear();
	for (int i = 0; i < path.size(); i++)
	{
		waypoints.push_back(path[i]->position);
		ids.push_back(path[i]->id);
	}
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

Node* Grid::NodeFromWorldPoint(Vector3 worldPoint, int cutoff)
{
	int closest = cutoff;
	int savedId = -1;
	for (auto [id, n] : m)
	{
		float distance = Vector3::Distance(n->position, worldPoint);
		if (distance < cutoff)
		{
			if (distance <= closest)
			{
				closest = distance;
				savedId = id;
			}
		}
	}
	if (savedId != -1)
		return m.find(savedId)->second;
	else
		return nullptr;
}

//std::vector<Vector3> Grid::optimizePath(std::vector<Node*> path)
//{
//	std::vector<Vector3> waypoints;
//	Vector2 directionOld = Vector2::Zero;
//
//	for (int i = 1; i < path.size(); i++)
//	{
//		// may have to change this to world coordinates instead of indexes. but it might work
//		Vector2 directionNew = Vector2(path[i - (int)1]->gridX - path[i]->gridX, path[i - (int)1]->gridY - path[i]->gridY);
//		if (directionNew != directionOld)
//		{
//			waypoints.push_back(path[i]->position);
//		}
//		directionOld = directionNew;
//	}
//
//
//	return waypoints;
//}

