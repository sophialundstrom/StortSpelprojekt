#pragma once
#include "Graphics.h"
#include "Drawable.h"
#include "Math.h"

struct DynamicPlane
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
	};

	UINT size = 0;
	UINT indexCount = 0;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	DynamicPlane(UINT subdivisions, UINT size);
	~DynamicPlane();

	void Draw();
};