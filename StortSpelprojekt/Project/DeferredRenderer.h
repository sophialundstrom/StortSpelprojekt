#pragma once
#include "Renderer.h"
#include "DirectXHelp.h"
#include "Settings.h"

class DeferredRenderer : public Renderer
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

	//SHADERS
	const std::string vs_path = "../x64/Debug/DeferredVertexShader.cso";
	ID3D11VertexShader* vertexShader = nullptr;

	const std::string ps_path = "../x64/Debug/DeferredPixelShader.cso";
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
			hr = Graphics::GetDevice().CreateTexture2D(&textureDesc, nullptr, &textures[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE TEXTURE2D");
				return;
			}
		}

		//RENDER TARGETS
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; //WE WANT TO ACCESS DATA AS A TEXTURE 2D
		rtvDesc.Format = textureDesc.Format;

		for (UINT i = 0; i < numTargets; ++i)
		{
			hr = Graphics::GetDevice().CreateRenderTargetView(textures[i], &rtvDesc, &rtvs[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE RENDER TARGET");
				return;
			}
		}

		//SHADER RESOURCE VIEW
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//WE WANT TO ACCESS DATA AS A TEXTURE 2D
		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MostDetailedMip = 0;					//ID
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;	//DEFAULTS TO -1

		for (UINT i = 0; i < numTargets; ++i)
		{
			hr = Graphics::GetDevice().CreateShaderResourceView(textures[i], &srvDesc, &srvs[i]);
			if FAILED(hr)
			{
				Print("FAILED TO CREATE SHADER RESOURCE VIEW");
				return;
			}
		}

		//RELEASE TEXTURES
		for (UINT i = 0; i < numTargets; ++i)
			textures[i]->Release();

		//SHADERS
		std::string byteCode;
		LoadVertexShader(vertexShader, vs_path, byteCode);
		LoadPixelShader(pixelShader, ps_path);

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = Graphics::GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT");
			return;
		}

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
		CreateStructuredBuffer(lights_buf, lights_buf_srv, sizeof(PointLight), MAX_LIGHTS * sizeof(PointLight));
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
		//DEACTIVATE WIREFRAME IF ENABLED
		if (Settings::UseWireframe())
			Graphics::DisableWireframe();

		//START RENDERING TO BACK BUFFER
		Graphics::BeginFrame();
		ImGUI::BeginFrame();

		//IMGUI
		ImGui::Begin("NORMALS");
		ImGui::Image(srvs[1], { 1600 / 5, 900 / 5 });
		ImGui::End();

		ImGui::Begin("WORLD POSITIONS");
		ImGui::Image(srvs[2], { 1600 / 5, 900 / 5 });
		ImGui::End();

		Settings::RenderUI();
		ShaderData::shadowMap.RenderAsImage();

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//SHADER RESOURCES
		Graphics::GetContext().PSSetShaderResources(0, numTargets, srvs);
		ShaderData::shadowMap.BindAsResource();
		Graphics::GetContext().PSSetShaderResources(8, 1, &lights_buf_srv);

		//BUFFERS
		UpdateBuffer(light_buf, ShaderData::lightData);
		BindBuffer(light_buf, Shader::PS);

		UpdateBuffer(camera_buf, ShaderData::cameraPosition);
		BindBuffer(camera_buf, Shader::PS, 1);

		UpdateBuffer(numLights_buf, ShaderData::numPointLights);
		BindBuffer(numLights_buf, Shader::PS, 2);

		UpdateBuffer(globalAmbient_buf, Settings::GlobalAmbient());
		BindBuffer(globalAmbient_buf, Shader::PS, 3);

		UpdateBuffer(lights_buf, ShaderData::pointLightsData, sizeof(PointLight) * ShaderData::numPointLights);

		//TOPOLOGY
		Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//INPUT LAYOUT
		Graphics::GetContext().IASetInputLayout(inputLayout);

		//DRAW
		Graphics::GetContext().IASetVertexBuffers(0, 1, &screenQuadBuffer, &stride, &offset);
		Graphics::GetContext().Draw(4, 0);

		//FINALLY RENDER TO BACK BUFFER
		ImGUI::EndFrame();
		Graphics::EndFrame();

		//REACTIVATE WIREFRAME IF ENABLED
		if (Settings::UseWireframe())
			Graphics::EnableWireframe();

		//RESET FOR NEXT FRAME
		SetRenderTargets();
	}

	void SetRenderTargets()
	{
		Graphics::GetContext().RSSetViewports(1, &Graphics::GetViewport());

		//RESET TEXTURES TO BE USED AS RENDER TARGETS
		ID3D11ShaderResourceView* nullSRVs[numTargets] = { nullptr };
		Graphics::GetContext().PSSetShaderResources(0, numTargets, nullSRVs);

		Graphics::GetContext().OMSetRenderTargets(numTargets, rtvs, &Graphics::GetDSV());

		for (UINT i = 0; i < numTargets; ++i)
			Graphics::GetContext().ClearRenderTargetView(rtvs[i], Graphics::GetBackgroundColor());

		Graphics::GetContext().ClearDepthStencilView(&Graphics::GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
};