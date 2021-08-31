#pragma once
#include "Renderer.h"
#include "ParticleSystem.h"

class ParticleRenderer : public Renderer
{
	//BUFFERS
	ID3D11Buffer* camera_buf = nullptr;
	ID3D11Buffer* extents_buf = nullptr;
	ID3D11Buffer* matrix_buf = nullptr;

	//SHADERS
	const std::string vs_path = "../x64/Debug/ParticleVertexShader.cso";
	ID3D11VertexShader* vertexShader = nullptr;

	const std::string gs_path = "../x64/Debug/ParticleGeometryShader.cso";
	ID3D11GeometryShader* geometryShader = nullptr;

	const std::string ps_path = "../x64/Debug/ParticlePixelShader.cso";
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ParticleRenderer()
	{
		//BUFFERS
		CreateBuffer(camera_buf);
		CreateBuffer(extents_buf);
		CreateBuffer(matrix_buf, sizeof(Matrix));

		//SHADERS
		std::string byteCode;
		LoadVertexShader(vertexShader, vs_path, byteCode);
		LoadGeometryShader(geometryShader, gs_path);
		LoadPixelShader(pixelShader, ps_path);

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"VELOCITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT");
			return;
		}
	}

	~ParticleRenderer()
	{
		//BUFFERS
		camera_buf->Release();
		extents_buf->Release();
		matrix_buf->Release();

		//SHADERS
		vertexShader->Release();
		geometryShader->Release();
		pixelShader->Release();

		//INPUT LAYOUT
		inputLayout->Release();
	}

	void Render()
	{
		//INPUT LAYOUT
		Graphics::GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, geometryShader, pixelShader);

		//BUFFERS
		UpdateBuffer(camera_buf, ShaderData::cameraPosition);
		BindBuffer(camera_buf, Shader::GS);

		UpdateBuffer(matrix_buf, ShaderData::cameraMatrix);
		BindBuffer(matrix_buf, Shader::GS, 1);

		for (auto& drawable : drawables)
		{
			auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
			if (!particleSystem)
				continue;

			UpdateBuffer(extents_buf, particleSystem->GetParticleExtents());
			BindBuffer(extents_buf, Shader::GS, 2);

			particleSystem->BindBuffer();
			particleSystem->DrawParticles();
		}
	}
};