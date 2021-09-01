#pragma once
#include "Graphics.h"
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

	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
public:
	Terrain(float size);
	~Terrain();

	void Bind();
};