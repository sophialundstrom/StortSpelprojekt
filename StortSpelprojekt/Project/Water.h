#pragma once
#include "DynamicPlane.h"
#include "Texture.h"
#include "PlaneWithNormals.h"

class Water : public Drawable
{
private:
	PlaneWithNormals* plane;
	Texture* texture;
	Texture* normalMap1;
	Texture* normalMap2;

public:
	Water() = default;
	Water(UINT size, UINT subdivisions = 0)
	{
		plane = new PlaneWithNormals(subdivisions, size);
		texture = new Texture("Textures/Ocean.png");
		normalMap1 = new Texture("Textures/NormalMaps/WaterNormalMap2Seamless.jpg");
		normalMap2 = new Texture("Textures/NormalMaps/WaterNormalMap3Seamless.jpg");
	}

	~Water()
	{
		delete plane;
		delete texture;
		delete normalMap1;
		delete normalMap2;
	}

	void Draw() const
	{		
		normalMap2->Bind(12);
		normalMap1->Bind(11);
		texture->Bind();
		plane->Draw();
	}
};