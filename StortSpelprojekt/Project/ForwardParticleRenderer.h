#pragma once

#include "ShaderData.h"
#include "ParticleSystem.h"

class ForwardParticleRenderer
{
private:
	//BUFFERS
	ID3D11Buffer* particleSize_buf = nullptr;
	ID3D11Buffer* matrix_buf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Debug/ParticleGeometryShader.cso";
	const std::string ps_path = "../x64/Debug/ForwardParticlePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Release/ParticleGeometryShader.cso";
	const std::string ps_path = "../x64/Release/ForwardParticlePixelShader.cso";
#endif
	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ForwardParticleRenderer()
	{
		//BUFFERS
		CreateBuffer(particleSize_buf);
		CreateBuffer(matrix_buf, sizeof(Matrix));

		//SHADERS
		std::string byteCode;
		LoadShader(vertexShader, vs_path, byteCode);
		LoadShader(geometryShader, gs_path);
		LoadShader(pixelShader, ps_path);

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"VELOCITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT");
			return;
		}
	}

	~ForwardParticleRenderer()
	{
		//BUFFERS
		particleSize_buf->Release();
		matrix_buf->Release();

		//SHADERS
		vertexShader->Release();
		geometryShader->Release();
		pixelShader->Release();

		//INPUT LAYOUT
		inputLayout->Release();
	}

	void Render(ParticleSystem* particleSystem)
	{
		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, geometryShader, pixelShader);

		//BUFFERS
		BindBuffer(ShaderData::Inst().cameraPosition_buf, Shader::GS);

		UpdateBuffer(matrix_buf, ShaderData::Inst().cameraMatrix);
		BindBuffer(matrix_buf, Shader::GS, 1);

		UpdateBuffer(particleSize_buf, particleSystem->GetParticleExtents());
		BindBuffer(particleSize_buf, Shader::GS, 2);

		//DRAW
		particleSystem->Draw();
	}
};