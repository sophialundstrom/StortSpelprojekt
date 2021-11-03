#pragma once
#include "Graphics.h"
#include "Texture.h"
#include "Math.h"
#include <map>
#include "DynamicPlane.h"

struct HeightMap
{
	Texture* texture;

	int width, height;
	struct Data
	{
		float x, y, z;
	};

	std::map<Vector2, float> data;

	HeightMap(const std::string& texture);

	~HeightMap() { delete texture; }
};

class Terrain
{
private:
	DynamicPlane* plane;

	Texture* blendMap;
	Texture* pathMap;
	Texture* textures[4];
	HeightMap* heightMap;
public:
	Terrain(UINT subdivisions = 0);
	~Terrain();

	HeightMap* GetHeightMap() { return heightMap; }
	void Draw() const;
};