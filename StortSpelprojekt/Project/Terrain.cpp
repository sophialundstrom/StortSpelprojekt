#include "Terrain.h"

#include "DirectXHelp.h"

Terrain::Terrain(float size)
{
	//VERTICES
	Vertex vertices[] =
	{
		{ {size, 0, size},		{1, 1} },
		{ {-size, 0, -size},	{0, 0} },
		{ {-size, 0, size},		{0, 1} },
		{ {size, 0, -size},		{1, 0} }
	};
	CreateVertexBuffer(vertexBuffer, sizeof(Vertex), ARRAYSIZE(vertices), &vertices);

	//INDICES
	UINT indices[]
	{ 3, 1, 0, 2 };
	indexCount = ARRAYSIZE(indices);
	CreateIndexBuffer(indexBuffer, ARRAYSIZE(indices), &indices);
}

Terrain::~Terrain()
{
	indexBuffer->Release();
	vertexBuffer->Release();
}

void Terrain::Draw()
{
	Graphics::Inst().GetContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	Graphics::Inst().GetContext().DrawIndexed(indexCount, 0, 0);

	Graphics::Inst().GetContext().IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
}