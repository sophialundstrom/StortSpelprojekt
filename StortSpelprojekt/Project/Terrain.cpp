#include "Terrain.h"
#include "DirectXHelp.h"
#include "stb_image.h"

HeightMap::HeightMap(const std::string& texture)
{
	std::string path = "Textures/" + texture + ".png";

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
			float finalValue = (float)value * 2.0f - 100.0f;

			data.emplace(Vector2((float)i - (width / 2) + 1, (float)j - (height / 2) + 1), finalValue);
		}
	}
}

Terrain::Terrain(UINT subdivisions)
{
	heightMap = new HeightMap("heightMap");
	UINT size = heightMap->width;
	
	plane = new DynamicPlane(subdivisions, heightMap->width);

	heightMap->texture->Bind(0, Shader::DS);

	blendMap = new Texture("Textures/blendMap.png", "BlendMap");
	blendMap->Bind(10);
	const std::string tx[3] = { "SandSeamless.jpg", "GrassSeamless.jpg", "RockSeamless.jpg" };
	for (UINT i = 0; i < 3; ++i)
		textures[i] = new Texture("Textures/" + tx[i], tx[i]);
}

Terrain::~Terrain()
{
	delete heightMap;
	delete blendMap;
	for (UINT i = 0; i < 3; ++i)
		delete textures[i];
	delete plane;
}

void Terrain::Draw() const
{

	for (UINT i = 0; i < 3; ++i)
		textures[i]->Bind(i + 1);

	plane->Draw();
}