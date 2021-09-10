#pragma once
#include "Graphics.h"
#include "Texture.h"
#include "Math.h"

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

	Texture heightMap;
public:
	Terrain(float size);
	~Terrain();

	void Draw() const;
};