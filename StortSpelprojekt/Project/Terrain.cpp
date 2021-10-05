#include "Terrain.h"

#include "DirectXHelp.h"
#include "stb_image.h"

HeightMap::HeightMap(const std::string& texture)
{
	std::string path = "Textures/" + texture + ".jpg";

	this->texture = new Texture(path, texture);

	int channels = 3;
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, channels);
	if (!image)
	{
		Print("FAILED TO LOAD HEIGHT MAP");
		return;
	}

	UINT k = 0;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			unsigned char* pixelOffset = image + (i + width * j) * channels;

			int value = pixelOffset[0];
			float finalValue = (float)value * 0.2f;

			data.emplace(Vector2(i - (width / 2) + 1, j - (height / 2) + 1), finalValue);
		}
	}
}

Terrain::Terrain(float size, UINT subdivisions)
{
	heightMap = new HeightMap("heightMap");
	size = heightMap->width;
	
	//TEST STUFF
	const UINT cells = pow(2, subdivisions);
	const UINT numTris = pow(2, subdivisions * 2 + 1);
	const float triSize = size / cells;
	const float texSize = 1.0 / cells;

	Vector3 position;
	Vector2 texCoords;

	std::vector<Vertex> vertices;
	for (UINT i = 0; i < cells + 1; ++i)
	{
		for (UINT j = 0; j < cells + 1; ++j)
		{
			position = { triSize * i - (size / 2) , 0, triSize * j - (size / 2)};
			texCoords = { texSize * i, texSize * j };

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
	CreateVertexBuffer(vertexBuffer, sizeof(Vertex), vertices.size()  * sizeof(Vertex), vertices.data());

	//INDICES
	indexCount = Indices.size();
	CreateIndexBuffer(indexBuffer, Indices.size(), Indices.data());

	heightMap->texture->Bind(0, Shader::DS);

	blendMap = new Texture("Textures/BlendMap.jpg", "BlendMap");

	const std::string tx[3] = { "Stone512.png", "water.png", "grass2seamless.png" };
	for (UINT i = 0; i < 3; ++i)
		textures[i] = new Texture("Textures/" + tx[i], tx[i]);
}

Terrain::~Terrain()
{
	delete heightMap;
	delete blendMap;
	for (UINT i = 0; i < 3; ++i)
		delete textures[i];
	indexBuffer->Release();
	vertexBuffer->Release();
}

void Terrain::Draw() const
{
	blendMap->Bind();
	for (UINT i = 0; i < 3; ++i)
		textures[i]->Bind(i + 1);

	Graphics::Inst().GetContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	Graphics::Inst().GetContext().DrawIndexed(indexCount, 0, 0);

	Graphics::Inst().GetContext().IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
}