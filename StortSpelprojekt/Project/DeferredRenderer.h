#pragma once
#include "ShaderData.h"

class DeferredRenderer
{
private:
	static const UINT numTargets = 7u;
	ID3D11RenderTargetView* rtvs[numTargets] = { nullptr };
	ID3D11ShaderResourceView* srvs[numTargets] = { nullptr };

	//SCREEN QUAD
	ID3D11Buffer* screenQuadBuffer = nullptr;

	const UINT stride = sizeof(DeferredVertex);
	const UINT offset = 0;

	struct DeferredVertex
	{
		Vector3 position;
		Vector2 texCoords;
	};

	//BUFFERS
	ID3D11Buffer* light_buf;
	ID3D11Buffer* camera_buf;
	ID3D11Buffer* numLights_buf;
	ID3D11Buffer* globalAmbient_buf;

	//STRUCTURED BUFFER
	ID3D11Buffer* lights_buf;
	ID3D11ShaderResourceView* lights_buf_srv;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/DeferredVertexShader.cso";
	const std::string ps_path = "../x64/Debug/DeferredPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/DeferredVertexShader.cso";
	const std::string ps_path = "../x64/Release/DeferredPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	DeferredRenderer(UINT width, UINT height)
	{
		HRESULT hr;

		//TEXTURE 2D
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;							// 32 BIT PER CHANNEL, FLOAT VALUES
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	//BIND AS RENDER TARGET & SHADER RESOURCE
		textureDesc.Usage = D3D11_USAGE_DEFAULT;										// GPU READ/WRITE
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.MiscFlags = 0;
		textureDesc.Height = height;
		textureDesc.Width = width;

		ID3D11Texture2D* textures[numTargets] = { nullptr };
		for (UINT i = 0; i < numTargets; ++i)
		{
			hr = Graphics::Inst().GetDevice().CreateTexture2D(&textureDesc, nullptr, &textures[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE TEXTURE2D", "DEFERRED RENDERER");
				return;
			}
		}
		Print("SUCCEEDED TO CREATE TEXTURE2D", "DEFERRED RENDERER");

		//RENDER TARGETS
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; //WE WANT TO ACCESS DATA AS A TEXTURE 2D
		rtvDesc.Format = textureDesc.Format;

		for (UINT i = 0; i < numTargets; ++i)
		{
			hr = Graphics::Inst().GetDevice().CreateRenderTargetView(textures[i], &rtvDesc, &rtvs[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE RENDER TARGET", "DEFERRED RENDERER");
				return;
			}
		}
		Print("SUCCEEDED TO CREATE RENDER TARGETS", "DEFERRED RENDERER");

		//SHADER RESOURCE VIEW
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//WE WANT TO ACCESS DATA AS A TEXTURE 2D
		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MostDetailedMip = 0;					//ID
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;	//DEFAULTS TO -1

		for (UINT i = 0; i < numTargets; ++i)
		{
			hr = Graphics::Inst().GetDevice().CreateShaderResourceView(textures[i], &srvDesc, &srvs[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE SHADER RESOURCE VIEW", "DEFERRED RENDERER");
				return;
			}
		}
		Print("SUCCEEDED TO CREATE SHADER RESOURCE VIEW", "DEFERRED RENDERER");

		//RELEASE TEXTURES
		for (UINT i = 0; i < numTargets; ++i)
			textures[i]->Release();

		//SHADERS
		std::string byteCode;
		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;
		if (!LoadShader(pixelShader, ps_path))
			return;
		Print("SUCCEEDED LOADING SHADERS", "DEFERRED RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "DEFERRED RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "DEFERRED RENDERER");

		//SCREEN QUAD
		DeferredVertex screenQuad[] =
		{
		   { {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },  // TOP RIGHT

		   { {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }, // BOTTOM RIGHT

		   { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f} }, // TOP LEFT

		   { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} } // BOTTOM LEFT
		};
		CreateVertexBuffer(screenQuadBuffer, sizeof(DeferredVertex), sizeof(DeferredVertex) * ARRAYSIZE(screenQuad), screenQuad);

		//BUFFERS
		CreateBuffer(light_buf, sizeof(DirectionalLight::Data));
		CreateBuffer(camera_buf);
		CreateBuffer(numLights_buf);
		CreateBuffer(globalAmbient_buf);
		CreateStructuredBuffer(lights_buf, lights_buf_srv, sizeof(PointLight::Data), MAX_LIGHTS * sizeof(PointLight::Data));

		Print("SUCCEEDED TO INITIALIZE DEFERRED RENDERER");
		Print("=======================================");
	}

	~DeferredRenderer()
	{
		for (UINT i = 0; i < numTargets; ++i)
		{
			rtvs[i]->Release();
			srvs[i]->Release();
		}

		screenQuadBuffer->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
		light_buf->Release();
		camera_buf->Release();
		lights_buf->Release();
		numLights_buf->Release();
		lights_buf_srv->Release();
		globalAmbient_buf->Release();
	}

	void Render()
	{
		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//SHADER RESOURCES
		Graphics::Inst().GetContext().PSSetShaderResources(0, numTargets, srvs);
		ShaderData::Inst().shadowMap.BindAsResource();
		Graphics::Inst().GetContext().PSSetShaderResources(8, 1, &lights_buf_srv);

		//BUFFERS
		UpdateBuffer(light_buf, ShaderData::Inst().lightData);
		BindBuffer(light_buf, Shader::PS);

		UpdateBuffer(camera_buf, ShaderData::Inst().cameraPosition);
		BindBuffer(camera_buf, Shader::PS, 1);

		UpdateBuffer(numLights_buf, ShaderData::Inst().numPointLights);
		BindBuffer(numLights_buf, Shader::PS, 2);

		UpdateBuffer(lights_buf, ShaderData::Inst().pointLightsData, sizeof(PointLight) * ShaderData::Inst().numPointLights);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//DRAW
		Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &screenQuadBuffer, &stride, &offset);
		Graphics::Inst().GetContext().Draw(4, 0);
	}

	void SetRenderTargets()
	{
		Graphics::Inst().GetContext().RSSetViewports(1, &Graphics::Inst().GetViewport());

		//RESET TEXTURES TO BE USED AS RENDER TARGETS
		ID3D11ShaderResourceView* nullSRVs[numTargets] = { nullptr };
		Graphics::Inst().GetContext().PSSetShaderResources(0, numTargets, nullSRVs);

		Graphics::Inst().GetContext().OMSetRenderTargets(numTargets, rtvs, &Graphics::Inst().GetDSV());

		for (UINT i = 0; i < numTargets; ++i)
			Graphics::Inst().GetContext().ClearRenderTargetView(rtvs[i], Graphics::Inst().GetBackgroundColor());

		Graphics::Inst().GetContext().ClearDepthStencilView(&Graphics::Inst().GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
};