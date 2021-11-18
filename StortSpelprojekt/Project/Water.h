#pragma once
#include "DynamicPlane.h"
#include "Texture.h"

class Water : public Drawable
{
private:
	DynamicPlane* plane;
	Texture* texture;
	Texture* normalMap;
public:
	Water() = default;
	Water(UINT size, UINT subdivisions = 0)
	{
		plane = new DynamicPlane(subdivisions, size);
		texture = new Texture("Textures/water.png");
		normalMap = new Texture("Textures/WaterNormalMap1.jpg");
	}

	~Water()
	{
		delete plane;
		delete texture;
		delete normalMap;
	}

	void Draw() const
	{
		normalMap->Bind(1);
		texture->Bind();
		plane->Draw();
	}
};