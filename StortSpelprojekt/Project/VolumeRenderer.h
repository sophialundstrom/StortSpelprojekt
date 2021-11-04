#pragma once
#include "ShaderData.h"
#include "BoundingVolumes.h"

class VolumeRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
	//INDEX BUFFERS
	ID3D11Buffer* sphereIndices = nullptr;
	ID3D11Buffer* sphereVertexBuffer = nullptr;

	ID3D11Buffer* boxIndices = nullptr;
	ID3D11Buffer* boxVertexBuffer = nullptr;

	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;

	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	}matrices;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/VolumeVertexShader.cso";
	const std::string forward_ps_path = "../x64/Debug/VolumePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/VolumeVertexShader.cso";
	const std::string forward_ps_path = "../x64/Release/VolumePixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//BLEND STATES
	ID3D11BlendState* blendState = nullptr; 

public:
	VolumeRenderer()
	{
		//INDEX BUFFERS
		CreateIndexBuffer(sphereIndices, SphereVolumeData::INDICES, SphereVolumeData::indices);
		CreateIndexBuffer(boxIndices, BoxVolumeData::INDICES, BoxVolumeData::indices);

		//VERTEX BUFFERS
		CreateVertexBuffer(sphereVertexBuffer, stride, SphereVolumeData::VERTICES * stride, SphereVolumeData::vertices);
		CreateVertexBuffer(boxVertexBuffer, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));

		//SHADERS
		std::string byteCode;

		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;


		if (!LoadShader(pixelShader, forward_ps_path))
			return;


		Print("SUCCEEDED LOADING SHADERS", "VOLUME RENDERER");

		//BLEND STATE
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0X0f;
		HRESULT hr = Graphics::Inst().GetDevice().CreateBlendState(&blendDesc, &blendState);


		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

		hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "VOLUME RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "VOLUME RENDERER");

		Print("SUCCEEDED TO INITIALIZE VOLUME RENDERER");
		Print("=======================================");
	}

	~VolumeRenderer()
	{
		sphereIndices->Release();
		boxIndices->Release();
		matricesBuf->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
		blendState->Release();
		boxIndices->Release();
		sphereVertexBuffer->Release();
		boxVertexBuffer->Release();
	}

	virtual void Render() override
	{
		if (drawables.empty())

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//BLEND STATE
		Graphics::Inst().GetContext().OMSetBlendState(blendState, nullptr, 0Xffffffff);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//BUFFER
		BindBuffer(matricesBuf);

		matrices.viewPerspective = ShaderData::Inst().cameraMatrix;

		for (auto& drawable : drawables)
		{
			auto volume = std::dynamic_pointer_cast<BoundingVolume>(drawable);
			if (!volume)
				continue;


			matrices.world = drawable->GetMatrix();
			UpdateBuffer(matricesBuf, matrices);

			switch (volume->type)
			{
			case VolumeType::BOX:
				Graphics::Inst().GetContext().IASetIndexBuffer(boxIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &boxVertexBuffer, &stride, &offset);
				Graphics::Inst().GetContext().DrawIndexed(BoxVolumeData::INDICES, 0, 0);
				break;

			case VolumeType::SPHERE:
				Graphics::Inst().GetContext().IASetIndexBuffer(sphereIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &sphereVertexBuffer, &stride, &offset);
				Graphics::Inst().GetContext().DrawIndexed(SphereVolumeData::INDICES, 0, 0);
				break;

			}
		}

		Graphics::Inst().GetContext().OMSetBlendState(nullptr, nullptr, 0Xffffffff);
	}

};