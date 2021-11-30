#include "Terrain.h"
#include "DirectXHelp.h"
#include "stb_image.h"
#include "ThreadPool.h"
#include "HardwareSupport.h"
//static std::mutex terrainLock;

HeightMap::HeightMap(const std::string& texture)
{
	std::string path = "Textures/" + texture + ".png";

	this->texture = new Texture(path, texture);

	int channels = 4;
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, channels);
	if (!image)
	{
		Print("FAILED TO LOAD HEIGHT MAP");
		return;
	}

	UINT k = 0;

	//ThreadPool tp(HardwareSupport::numThreads);
	//std::atomic<int> numChunks = 4;

	//for (int i = 0; i < numChunks.load(); i++)
	//{
	//	tp.Enqueue([=, &numChunks]
	//		{
	//			for (int w = i * 500; w < width; w++)
	//			{
	//				for (int j = 0; j < height; ++j)
	//				{
	//					unsigned char* pixelOffset = image + (w + width * j) * channels;

	//					int value = pixelOffset[0];
	//					float finalValue = (float)value * 2.0f - 100.0f;

	//					terrainLock.lock();
	//					data.emplace(Vector2((float)w - (width / 2) + 1, (float)j - (height / 2) + 1), finalValue);
	//					terrainLock.unlock();
	//				}
	//			}
	//			numChunks.fetch_sub(1);
	//			terrainLock.lock();
	//			PrintNumber(numChunks.load());
	//			terrainLock.unlock();
	//		});
	//}

	//while (numChunks.load() != 0);

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

	pathMap = new Texture("Textures/PathBlendMap.png", "PathMap");

	const std::string tx[4] = { "SandSeamless.jpg", "GrassSeamless.jpg", "RockSeamless.jpg", "Gravel.jpg"};
	for (UINT i = 0; i < 4; ++i)
		textures[i] = new Texture("Textures/" + tx[i], tx[i]);
}

Terrain::~Terrain()
{
	delete heightMap;
	delete blendMap;
	delete pathMap;
	for (UINT i = 0; i < 4; ++i)
		delete textures[i];
	delete plane;
}

float Terrain::SampleAverage(float x, float z)
{
	const int lowX = (int)std::floor(x);
	const int highX = (int)std::ceil(x);
	const float Xdecimal = x - lowX;

	const int lowZ = (int)std::floor(z);
	const int highZ = (int)std::ceil(z);
	const float Zdecimal = z - lowZ;

	const float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
	const float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
	const float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
	const float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

	return H1 + H2 + H3 + H4;
}

void Terrain::Draw() const
{
	blendMap->Bind(0);
	pathMap->Bind(1);
	for (UINT i = 0; i < 4; ++i)
		textures[i]->Bind(i + 2);

	plane->Draw();
}