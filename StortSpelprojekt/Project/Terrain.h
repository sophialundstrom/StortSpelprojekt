#pragma once
#include "Graphics.h"
#include "Texture.h"
#include "Math.h"
#include <map>

struct HeightMap
{
	Texture* texture;

	int width, height;
	struct Data
	{
		float x, y, z;
	};

	std::map<Vector2, float> data;

	HeightMap(const std::string& texture);

	~HeightMap() { delete texture; }
};

class Terrain
{
private:
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
	};

	UINT indexCount = 0;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	HeightMap* heightMap;
public:
	Terrain(float size, UINT subdivisions = 1);
	~Terrain();

	HeightMap* GetHeightMap() { return heightMap; }
	void Draw() const;
};