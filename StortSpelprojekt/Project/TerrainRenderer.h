#pragma once
#include "ShaderData.h"
#include "Terrain.h"

class TerrainRenderer
{
private:
	//BUFFERS
	ID3D11Buffer* matrixBuf = nullptr;

	ID3D11Buffer* tesselationBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Debug/TerrainHullShader.cso";
	const std::string LOD_hs_path = "../x64/Debug/LODTerrainHullShader.cso";

	const std::string ds_path = "../x64/Debug/TerrainDomainShader.cso";
	const std::string LOD_ds_path = "../x64/Debug/LODTerrainDomainShader.cso";

	const std::string gs_path = "../x64/Debug/TerrainGeometryShader.cso";

	const std::string deferred_ps_path = "../x64/Debug/DeferredTerrainPixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardTerrainPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Release/TerrainHullShader.cso";
	const std::string LOD_hs_path = "../x64/Release/LODTerrainHullShader.cso";

	const std::string ds_path = "../x64/Release/TerrainDomainShader.cso";

	const std::string gs_path = "../x64/Release/TerrainGeometryShader.cso";

	const std::string deferred_ps_path = "../x64/Release/DeferredTerrainPixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardTerrainPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11HullShader* hullShader = nullptr;
	ID3D11DomainShader* domainShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	TerrainRenderer(RenderMethod method, float tesselationAmount = 10)
	{
		//BUFFER
		CreateBuffer(matrixBuf, sizeof(Matrix));
		BindBuffer(matrixBuf, Shader::DS);

		//SHADERS
		std::string byteCode;
		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;

		//EDITOR (MAX TESSELATION EVERYWHERE)
		if (method == FORWARD)
		{
			//FIXED TESSELATION AMOUNT
			CreateBuffer(tesselationBuf);
			UpdateBuffer(tesselationBuf, tesselationAmount);
			BindBuffer(tesselationBuf, Shader::HS);

			if (!LoadShader(hullShader, hs_path))
				return;

			if (!LoadShader(domainShader, ds_path))
				return;

			if (!LoadShader(pixelShader, forward_ps_path))
				return;
		}

		//IN-GAME
		else
		{
			CreateBuffer(tesselationBuf);
			UpdateBuffer(tesselationBuf, tesselationAmount);
			BindBuffer(tesselationBuf, Shader::HS);

			if (!LoadShader(hullShader, hs_path))
				return;

			if (!LoadShader(domainShader, ds_path))
				return;

			if (!LoadShader(geometryShader, gs_path))
				return;

			if (!LoadShader(pixelShader, deferred_ps_path))
				return;
		}
		Print("SUCCEEDED LOADING SHADERS", "TERRAIN RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "TERRAIN RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "TERRAIN RENDERER");

		Print("SUCCEEDED TO INITIALIZE TERRAIN RENDERER");
	}

	~TerrainRenderer()
	{
		if (tesselationBuf)
			tesselationBuf->Release();
		matrixBuf->Release();

		vertexShader->Release();
		hullShader->Release();
		domainShader->Release();
		if (geometryShader)
			geometryShader->Release();
		pixelShader->Release();

		inputLayout->Release();
	}

	void Render(const Terrain& terrain)
	{
		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		//SHADERS
		BindShaders(vertexShader, hullShader, domainShader, geometryShader, pixelShader);

		//BUFFER(S)
		UpdateBuffer(matrixBuf, ShaderData::Inst().cameraMatrix);

		//DRAW
		terrain.Draw();
	}
};