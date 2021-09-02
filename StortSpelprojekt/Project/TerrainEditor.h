#pragma once
#include "Editor.h"
#include "Terrain.h"

class TerrainEditor : public Editor
{
private:
	Terrain terrain;
public:
	TerrainEditor()
		:terrain(100)
	{

	}
};