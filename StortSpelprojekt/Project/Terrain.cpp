#include "Terrain.h"

#include "DirectXHelp.h"

Terrain::Terrain(float size)
	:heightMap("Textures/TerrainHeightMap.png")
{
	//VERTICES
	Vertex vertices[] =
	{
		{ {size, 0, size},		{1, 1} },
		{ {-size, 0, -size},	{0, 0} },
		{ {-size, 0, size},		{0, 1} },
		{ {size, 0, -size},		{1, 0} }
	};
	CreateVertexBuffer(vertexBuffer, sizeof(Vertex), ARRAYSIZE(vertices) * sizeof(Vertex), &vertices);

	//INDICES
	UINT indices[] =
	{ 0, 3, 1, 0, 1, 2 };
	indexCount = ARRAYSIZE(indices);
	CreateIndexBuffer(indexBuffer, ARRAYSIZE(indices), &indices);

	heightMap.Bind(0, Shader::DS);
}

Terrain::~Terrain()
{
	indexBuffer->Release();
	vertexBuffer->Release();
}

void Terrain::Draw() const
{
	heightMap.Bind();

	Graphics::Inst().GetContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	Graphics::Inst().GetContext().DrawIndexed(indexCount, 0, 0);

	Graphics::Inst().GetContext().IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
}