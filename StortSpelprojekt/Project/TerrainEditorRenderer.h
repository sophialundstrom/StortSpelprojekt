#pragma once

#include "Terrain.h"
#include "DirectXHelp.h"

class TerrainEditorRenderer
{
private:
	const std::string vs_path = "";
	ID3D11VertexShader* vertexShader = nullptr;

	const std::string ps_path = "";
	ID3D11PixelShader* pixelShader = nullptr;
public:
	TerrainEditorRenderer()
	{

	}

	void Render(const Terrain& terrain)
	{
		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//BUFFER
	}
};