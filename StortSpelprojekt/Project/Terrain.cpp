#include "Terrain.h"

#include "DirectXHelp.h"

Terrain::Terrain(float size, UINT subdivisions)
	:heightMap("Textures/TerrainHeightMap.png")
{
	//TEST STUFF
	const UINT cells = pow(2, subdivisions);
	const UINT numTris = pow(2, subdivisions * 2 + 1);
	const float triSize = size / cells;
	const float texSize = 1.0 / cells;

	Vector3 position;
	Vector2 texCoords;

	std::vector<Vertex> tempVertices;
	for (UINT i = 0; i < cells + 1; ++i)
	{
		for (UINT j = 0; j < cells + 1; ++j)
		{
			position = { triSize * i, 0, triSize * j };
			texCoords = { texSize * i, texSize * j };

			tempVertices.emplace_back(Vertex{ position, texCoords });
		}
	}

	const UINT offset = cells + 1;
	std::vector<UINT> tempIndex;
	for (UINT i = 0; i < cells; ++i)
	{
		for (UINT j = 0; j < cells; ++j)
		{
			tempIndex.emplace_back(i * offset + j);
			tempIndex.emplace_back(j + (i * offset) + 1);
			tempIndex.emplace_back(j + 1 + offset + offset * i);
			tempIndex.emplace_back(j + 1 + offset + offset * i);
			tempIndex.emplace_back(offset + i * offset + j);
			tempIndex.emplace_back(i * offset + j);
		}
	}

	////VERTICES
	CreateVertexBuffer(vertexBuffer, sizeof(Vertex), tempVertices.size()  * sizeof(Vertex), tempVertices.data());

	//INDICES
	indexCount = tempIndex.size();
	CreateIndexBuffer(indexBuffer, tempIndex.size(), tempIndex.data());

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