#pragma once
#include "Graphics.h"
#include "ImGui.h"

class ShadowMap
{
private:
	ID3D11ShaderResourceView* srv;
	ID3D11DepthStencilView* dsv;
	D3D11_VIEWPORT viewport = {};
	UINT size;
public:
	ShadowMap() = default;
	ShadowMap(UINT size)
		:size(size)
	{
		HRESULT hr;

		//VIEWPORT
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)size;
		viewport.Height = (float)size;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//TEXTURE
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = size;
		textureDesc.Height = size;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;									//TO BE ABLE DO BIND AS DEPTH STENCIL, 24 BIT IN RED CHANNEL(DEPTH), 8 IN GREEN(STENCIL)
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;											//GPU WRITE/READ
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;		//USING AS DEPTH STENCIL & SHADER RESOURCE
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* texture = nullptr;
		hr = Graphics::Inst().GetDevice().CreateTexture2D(&textureDesc, nullptr, &texture);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE TEXTURE 2D");
			return;
		}

		//DEPTH STENCIL VIEW
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;										//24 BIT DEPTH, 8 BIT STENCIL
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;								//ACCESING AS TEXTURE2D
		//dsvDesc.Texture2D.MipSlice = 0;
		hr = Graphics::Inst().GetDevice().CreateDepthStencilView(texture, &dsvDesc, &dsv);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE DEPTH STENCIL VIEW");
			return;
		}

		//SHADER RESOURCE VIEW
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;									//24 BIT IN RED CHANNEL, REST WILL BE UNUSED
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;								//ACCESING AS TEXTURE2D
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = Graphics::Inst().GetDevice().CreateShaderResourceView(texture, &srvDesc, &srv);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE SHADER RESOURCE VIEW");
			return;
		}

		texture->Release();
	}

	void BindAsRenderTarget()
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;										//REPLACE BOUND POSITION WITH NULL TO BE ABLE TO BE BOUND AS DEPTH STENCIL VIEW
		Graphics::Inst().GetContext().PSSetShaderResources(7, 1, &nullSRV);

		Graphics::Inst().GetContext().RSSetViewports(1, &viewport);

		ID3D11RenderTargetView* nullRTV = nullptr;											//NULL RENDER TARGET WONT OUTPUT ANYTHING, BUT SCENE WILL STILL BE "RENDERED" WITH OUT DEPTH STENCIL VIEW
		Graphics::Inst().GetContext().OMSetRenderTargets(1, &nullRTV, dsv);

		Graphics::Inst().GetContext().ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void BindAsResource() { Graphics::Inst().GetContext().PSSetShaderResources(7, 1, &srv); }

	void RenderAsImage()
	{
		ImGui::Begin("SHADOW MAP");
		ImGui::Image(srv, { size / 15.0f, size / 15.0f });
		ImGui::End();
	}

	void ShutDown()
	{
		srv->Release();
		dsv->Release();
	}
};