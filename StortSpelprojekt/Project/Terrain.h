#pragma once
#include "Graphics.h"
#include "Math.h"

struct HeightMap
{
	UINT width, height;
	Vector3* data;
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
public:
	Terrain(float size);
	~Terrain();

	void Draw();
};