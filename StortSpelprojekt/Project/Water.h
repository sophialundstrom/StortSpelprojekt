#pragma once
#include "DynamicPlane.h"
#include "Texture.h"

class Water : public Drawable
{
private:
	DynamicPlane* plane;
	Texture* texture;
public:
	Water() = default;
	Water(UINT size, UINT subdivisions = 0)
	{
		plane = new DynamicPlane(subdivisions, size);
		texture = new Texture("Textures/water.png");
	}

	~Water()
	{
		delete plane;
		delete texture;
	}

	void Draw() const
	{
		texture->Bind();
		plane->Draw();
	}
};