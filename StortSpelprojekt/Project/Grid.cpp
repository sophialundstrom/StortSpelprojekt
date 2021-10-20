#include "Grid.h"
const int size = 32;

void Grid::CreateGrid(std::map<std::string, std::shared_ptr<Drawable>> &drawable)
{
	//const int p = gridSizeX * gridSizeY;
	grid = new Node[gridSizeX * gridSizeY];
	Vector3 worldBottomLeft = position - (Vector3::Right * gridWorldSize.x / 2) - (Vector3::Forward * gridWorldSize.y / 2); // check values of up and right

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			Vector3 worldPoint = worldBottomLeft + Vector3::Right * (x * nodeDiameter * nodeRadius) + Vector3::Forward * (y * nodeDiameter * nodeRadius);
			for (auto& [name, drawable] : drawable)
			{
				if (Vector3::Distance(worldPoint, drawable->GetPosition()) < 4.0f)
				{
					int i = x * gridSizeY + y;

					BoundingSphere tmpSphere = { {drawable->GetPosition()}, {1.0f} }; // tempporary solution to a colliding issue
					if (grid[i].BSphere.GetBounds().Intersects(tmpSphere.GetBounds()))
					{
						if (name != "RainingGATOS")
						{
							grid[i].walkable = false;
							grid[i].position = worldPoint;
							Print(std::string("colliding: "));
						}
					}
				}
			}
		}
	}
}

Grid::Grid()
	:Transform(Vector3::Zero, Vector3::Zero, Vector3::Zero)
{
	nodeRadius = 0.5f;
	gridWorldSize = Vector2(32,32);
	nodeDiameter = nodeRadius * 2;
	gridSizeX = roundToInt(gridWorldSize.x/ nodeDiameter);
	gridSizeY = roundToInt(gridWorldSize.y / nodeDiameter);
}
