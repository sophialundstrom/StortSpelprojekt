#pragma once
#include "Graphics.h"
#include "Drawable.h"
#include "Math.h"
#include "DirectXHelp.h"

struct PlaneWithNormals
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	struct Vertex
	{
		Vector3 position;
		Vector2 texCoords;
		Vector3 normal;
		Vector3 tanget;
	};

	UINT size = 0;
	UINT indexCount = 0;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	PlaneWithNormals(UINT subdivisions, UINT size);
	~PlaneWithNormals();

	void Draw();
};