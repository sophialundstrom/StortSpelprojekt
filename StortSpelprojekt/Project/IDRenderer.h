#pragma once
#pragma once
#include "ShaderData.h"
#include "Model.h"

class IDRenderer : public Renderer
{
private:
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
	const std::string ps_path = "../x64/Debug/IDPixelShader.cso";

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//RENDER TARGET
	ID3D11RenderTargetView* idRTV;
	ID3D11Texture2D* idTexture;
	ID3D11Texture2D* idTextureData;

public:
	IDRenderer()
	{
		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));
		CreateBuffer(idBuffer);

		//SHADERS
		std::string byteCode;

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

		D3D11_TEXTURE2D_DESC textDesc;
		textDesc.Width = Graphics::Inst().GetViewport().Width;
		textDesc.Height = Graphics::Inst().GetViewport().Height;
		textDesc.MipLevels = 1;
		textDesc.ArraySize = 1;
		textDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
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

		textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

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
		matricesBuf->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
		idRTV->Release();
		idTexture->Release();
	}

	virtual void Render() override
	{
		if (drawables.empty())
			return;

		BeginFrame();

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		matrices.viewPerspective = shaderData.cameraMatrix;

		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;
			UpdateBuffer(idBuffer, model->GetID());
			BindBuffer(idBuffer, Shader::VS, 1);

			matrices.world = model->GetMatrix();

			UpdateBuffer(matricesBuf, matrices);
			BindBuffer(matricesBuf);

			model->Draw(false, false);
		}
	}

	void BeginFrame()
	{
		auto &context = Graphics::Inst().GetContext();

		context.OMSetRenderTargets(1, &idRTV, &Graphics::Inst().GetDSV());
		context.ClearRenderTargetView(idRTV, Graphics::Inst().GetBackgroundColor());
		context.ClearDepthStencilView(&Graphics::Inst().GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	int GetObjectID(int xPix, int yPix)
	{
		D3D11_BOX pixel;
		pixel.left = xPix;
		pixel.right = xPix + 1;
		pixel.top = yPix;
		pixel.bottom = yPix + 1;

		//Graphics::Inst().GetContext().CopyResource(idTextureData, idTexture);
		//D3D11_TEXTURE2D_DESC textureDesc;
		//idTextureData->GetDesc(&textureDesc);
		//D3D11_MAPPED_SUBRESOURCE mappedResource;
		//D3D11_SUBRESOURCE_DATA resourceData;

		//HRESULT hr = Graphics::Inst().GetContext().Map(idTextureData, (0, 0, 0, 0) , D3D11_MAP_READ, 0, &mappedResource);
		//if FAILED(hr)
		//{
		//	Print("FAILED TO MAP SUBRESOURCE", "ID RENDERER::COPYING TEXTURE");
		//}

		//resourceData.pSysMem = new BYTE[mappedResource.RowPitch * textureDesc.Height];
		//memcpy(const_cast<void*>(resourceData.pSysMem), mappedResource.pData, mappedResource.RowPitch* textureDesc.Height);

		//Graphics::Inst().GetContext().Unmap(idTextureData, 0);
		////+(yPix * 4 * sizeof(UINT)) + (xPix * 4 * sizeof(UINT))

		//int id = reinterpret_cast<int>(resourceData.pSysMem);
		return 1;
	}
};