#pragma once
#include "ShaderData.h"
#include "ParticleSystem.h"

template <RenderMethod method>
class ParticleRenderer : public Renderer
{
	//BUFFERS
	ID3D11Buffer* extentsBuf = nullptr;
	ID3D11Buffer* matrixBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Debug/ParticleGeometryShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredParticlePixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardParticlePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Release/ParticleGeometryShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredParticlePixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardParticlePixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ParticleRenderer()
	{
		//BUFFERS
		CreateBuffer(extentsBuf);
		CreateBuffer(matrixBuf, sizeof(Matrix));

		//SHADERS
		std::string byteCode;
		LoadShader(vertexShader, vs_path, byteCode);
		LoadShader(geometryShader, gs_path);

		if (method == FORWARD)
			LoadShader(pixelShader, forward_ps_path);
		else
			LoadShader(pixelShader, deferred_ps_path);

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

	~ParticleRenderer()
	{
		extentsBuf->Release();
		matrixBuf->Release();
		vertexShader->Release();
		geometryShader->Release();
		pixelShader->Release();
		inputLayout->Release();
	}

	void Render()
	{
		if (drawables.empty())
			return;

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, geometryShader, pixelShader);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		//BUFFERS
		BindBuffer(shaderData.cameraPositionBuf, Shader::GS);

		UpdateBuffer(matrixBuf, shaderData.cameraMatrix);
		BindBuffer(matrixBuf, Shader::GS, 1);

		for (auto& drawable : drawables)
		{
			auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
			if (!particleSystem)
				continue;

			UpdateBuffer(extentsBuf, particleSystem->GetParticleExtents());
			BindBuffer(extentsBuf, Shader::GS, 2);

			particleSystem->Draw();
		}
	}
};