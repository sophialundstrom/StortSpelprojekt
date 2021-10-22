#pragma once
#include "Graphics.h"
#include "Texture.h"
#include <vector>
#include "Math.h"

class LoadingScreen
{
private:

	struct minVertex {
		minVertex(const Vector3& pos, const Vector2& uv)
		{
			position = pos;
			UV = uv;
		};

		Vector3 position;
		Vector2 UV;
	};
	minVertex quad[4]{
		minVertex(Vector3(1,1,0), Vector2(1,0)), 
		minVertex(Vector3(1,-1,0), Vector2(1,1)),
		minVertex(Vector3(-1,1,0), Vector2(0,0)),
		minVertex(Vector3(-1,-1,0), Vector2(0,1))
	};

	Texture* texture;
	std::vector<std::string> backgrounds{ "LoadingScreen.png" };
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampler;
	ID3D11Buffer* vBuffer;

public:
	LoadingScreen();
	~LoadingScreen() {
		vShader->Release();
		pShader->Release();
		layout->Release();
		delete texture;
		sampler->Release();
	};

	void Draw();

};

void RunLoadingScreen();