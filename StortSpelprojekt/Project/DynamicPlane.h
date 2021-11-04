#pragma once
#include "Graphics.h"
#include "DirectXHelp.h"
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

	DynamicPlane(UINT subdivisions, UINT size)
	{
		const UINT cells = pow(2, subdivisions);
		const UINT numTris = pow(2, subdivisions * 2 + 1);
		const float triSize = (float)size / (float)cells;
		const float texSize = 1.0f / (float)cells;

		Vector3 position;
		Vector2 texCoords;

		std::vector<Vertex> vertices;
		for (UINT i = 0; i < cells + 1; ++i)
		{
			for (UINT j = 0; j < cells + 1; ++j)
			{
				position = { triSize * i - (size / 2) , 0, triSize * j - (size / 2) };
				texCoords = { texSize * i + (1.0f / size), texSize * j };

				vertices.emplace_back(Vertex{ position, texCoords });
			}
		}

		const UINT offset = cells + 1;
		std::vector<UINT> Indices;
		for (UINT i = 0; i < cells; ++i)
		{
			for (UINT j = 0; j < cells; ++j)
			{
				Indices.emplace_back(i * offset + j);
				Indices.emplace_back(j + (i * offset) + 1);
				Indices.emplace_back(j + 1 + offset + offset * i);
				Indices.emplace_back(j + 1 + offset + offset * i);
				Indices.emplace_back(offset + i * offset + j);
				Indices.emplace_back(i * offset + j);
			}
		}

		//VERTICES
		CreateVertexBuffer(vertexBuffer, sizeof(Vertex), vertices.size() * sizeof(Vertex), vertices.data());

		//INDICES
		indexCount = Indices.size();
		CreateIndexBuffer(indexBuffer, Indices.size(), Indices.data());
	}

	~DynamicPlane()
	{
		indexBuffer->Release();
		vertexBuffer->Release();
	}

	void Draw()
	{
		Graphics::Inst().GetContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		Graphics::Inst().GetContext().DrawIndexed(indexCount, 0, 0);

		Graphics::Inst().GetContext().IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
};