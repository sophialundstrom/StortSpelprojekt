#pragma once
#include "ShaderData.h"
#include "Water.h"

class WaterRenderer
{
private:
	//BUFFERS
	ID3D11Buffer* matrixBuf = nullptr;
	ID3D11Buffer* lightBuf = nullptr;
	ID3D11Buffer* tesselationBuf = nullptr;
	ID3D11Buffer* timeBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Debug/TerrainHullShader.cso";

	const std::string ds_path = "../x64/Debug/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Debug/TerrainGeometryShader.cso";

	const std::string ps_path = "../x64/Debug/WaterPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Release/TerrainHullShader.cso";

	const std::string ds_path = "../x64/Release/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Release/TerrainGeometryShader.cso";

	const std::string ps_path = "../x64/Release/WaterPixelShader.cso";
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
	WaterRenderer(float tesselationAmount = 50)
	{
		//BUFFERS
		CreateBuffer(matrixBuf, sizeof(Matrix));
		CreateBuffer(lightBuf, sizeof(Matrix));
		CreateBuffer(timeBuf);
	
		//SHADERS
		std::string byteCode;
		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;

		CreateBuffer(tesselationBuf);
		UpdateBuffer(tesselationBuf, tesselationAmount);

		if (!LoadShader(hullShader, hs_path))
			return;

		if (!LoadShader(domainShader, ds_path))
			return;

		if (!LoadShader(geometryShader, gs_path))
			return;

		if (!LoadShader(pixelShader, ps_path))
			return;

		Print("SUCCEEDED LOADING SHADERS", "WATER RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "WATER RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "WATER RENDERER");

		Print("SUCCEEDED TO INITIALIZE WATER RENDERER");
		Print("=======================================");
	}

	~WaterRenderer()
	{
		if (tesselationBuf)
			tesselationBuf->Release();
		matrixBuf->Release();
		timeBuf->Release();
		lightBuf->Release();
		vertexShader->Release();
		hullShader->Release();
		domainShader->Release();
		if (geometryShader)
			geometryShader->Release();
		pixelShader->Release();
		inputLayout->Release();
	}

	void Render(const Water& water)
	{
		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		//SHADERS
		BindShaders(vertexShader, hullShader, domainShader, geometryShader, pixelShader);

		//BUFFER(S)
		UpdateBuffer(lightBuf, ShaderData::Inst().lightMatrix);
		BindBuffer(lightBuf, Shader::PS);

		UpdateBuffer(matrixBuf, ShaderData::Inst().cameraMatrix);
		BindBuffer(matrixBuf, Shader::DS);

		BindBuffer(tesselationBuf, Shader::HS);

		UpdateBuffer(timeBuf, Time::Get());
		BindBuffer(timeBuf, Shader::DS, 1);

		//DRAW
		water.Draw();
	}
};