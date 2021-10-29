#pragma once
#pragma once
#include "ShaderData.h"
#include "Model.h"
#include <stdio.h>
#include <stdlib.h>
#include "BoundingVolumes.h"

class IDRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;

	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;
	ID3D11Buffer* idBuffer = nullptr;

	//SHADER PATHS
	const std::string vs_path = "../x64/Debug/IDVertexShader.cso";
	const std::string volume_vs_path = "../x64/Debug/VolumeIDVertexShader.cso";
	const std::string ps_path = "../x64/Debug/IDPixelShader.cso";

	//SHADERS
	ID3D11VertexShader* volumeVertexShader = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11InputLayout* volumeInputLayout = nullptr;
	ID3D11InputLayout* currentLayout = nullptr;

	//RENDER TARGET
	ID3D11RenderTargetView* idRTV;
	ID3D11Texture2D* idTexture;
	ID3D11Texture2D* idTextureData;

	//BOUNDING VOLUMES
	ID3D11Buffer* sphereIndices = nullptr;
	ID3D11Buffer* sphereVertexBuffer = nullptr;

	ID3D11Buffer* boxIndices = nullptr;
	ID3D11Buffer* boxVertexBuffer = nullptr;

public:
	IDRenderer()
	{

		//INDEX BUFFERS
		CreateIndexBuffer(sphereIndices, SphereVolumeData::INDICES, SphereVolumeData::indices);
		CreateIndexBuffer(boxIndices, BoxVolumeData::INDICES, BoxVolumeData::indices);

		//VERTEX BUFFERS
		CreateVertexBuffer(sphereVertexBuffer, stride, SphereVolumeData::VERTICES * stride, SphereVolumeData::vertices);
		CreateVertexBuffer(boxVertexBuffer, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));
		CreateBuffer(idBuffer);

		//SHADERS
		std::string byteCode;
		std::string vByteCode;

		if (!LoadShader(volumeVertexShader, volume_vs_path, vByteCode))
			return;
		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;
		if (!LoadShader(pixelShader, ps_path))
			return;
		Print("SUCCEEDED LOADING SHADERS", "ID RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "ID RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "ID RENDERER");

		D3D11_INPUT_ELEMENT_DESC vInputDesc[] =
		{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

		hr = Graphics::Inst().GetDevice().CreateInputLayout(vInputDesc, ARRAYSIZE(vInputDesc), vByteCode.c_str(), vByteCode.length(), &volumeInputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "ID RENDERER (VOLUME)");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "ID RENDERER (VOLUME)");

		D3D11_TEXTURE2D_DESC textDesc = {};
		textDesc.Width = (UINT)Graphics::Inst().GetViewport().Width;
		textDesc.Height = (UINT)Graphics::Inst().GetViewport().Height;
		textDesc.MipLevels = 1;
		textDesc.ArraySize = 1;
		textDesc.Format = DXGI_FORMAT_R32_UINT;
		textDesc.SampleDesc.Count = 1;
		textDesc.SampleDesc.Quality = 0;
		textDesc.Usage = D3D11_USAGE_DEFAULT;
		textDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		textDesc.CPUAccessFlags = 0;
		textDesc.MiscFlags = 0;

		hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTexture);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
		}

		textDesc.Width = 1;
		textDesc.Height = 1;
		textDesc.BindFlags = 0;
		textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		textDesc.Usage = D3D11_USAGE_STAGING;

		hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTextureData);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
		}

		hr = Graphics::Inst().GetDevice().CreateRenderTargetView(idTexture, nullptr, &idRTV);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE RENDER TARGET VIEW", "ID RENDERER");
		}


		Print("SUCCEEDED TO INITIALIZE ID RENDERER");
		Print("=======================================");
	}

	~IDRenderer()
	{
		idBuffer->Release();
		matricesBuf->Release();
		volumeVertexShader->Release();
		vertexShader->Release();
		pixelShader->Release();
		volumeInputLayout->Release();
		inputLayout->Release();
		idRTV->Release();
		idTexture->Release();
		sphereIndices->Release();
		sphereVertexBuffer->Release();
		boxIndices->Release();
		boxVertexBuffer->Release();
		idTextureData->Release();
	}

	virtual void Render() override
	{
		if (drawables.empty())
			return;

		BeginFrame();

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		matrices.viewPerspective = shaderData.cameraMatrix;

		for (auto& drawable : drawables)
		{
			UpdateBuffer(idBuffer, drawable->GetID());
			BindBuffer(idBuffer, Shader::VS, 1);

			matrices.world = drawable->GetMatrix();

			UpdateBuffer(matricesBuf, matrices);
			BindBuffer(matricesBuf);

			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (model)
			{
				if (currentLayout != inputLayout)
				{
					currentLayout = inputLayout;
					Graphics::Inst().GetContext().IASetInputLayout(currentLayout);
					BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);
				}
				model->Draw(false, false);
				continue;
			}

			auto volume = std::dynamic_pointer_cast<BoundingVolume>(drawable);
			if (volume)
			{
				if (currentLayout != volumeInputLayout)
				{
					currentLayout = volumeInputLayout;
					Graphics::Inst().GetContext().IASetInputLayout(currentLayout);
					BindShaders(volumeVertexShader, nullptr, nullptr, nullptr, pixelShader);
				}
			
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
				continue;
			}

		}

		currentLayout = nullptr;
	}

	void BeginFrame()
	{
		auto &context = Graphics::Inst().GetContext();

		context.OMSetRenderTargets(1, &idRTV, &Graphics::Inst().GetDSV());
		context.ClearRenderTargetView(idRTV, Graphics::Inst().GetBackgroundColor());
		context.ClearDepthStencilView(&Graphics::Inst().GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	int GetObjectID(int xPix, int yPix);
};