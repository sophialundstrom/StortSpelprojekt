#include "Editor.h"

void Editor::UpdateViewport(float width, float height)
{
	HRESULT hr;

	//VIEWPORT
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//RENDER TARGET TEXTURE
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = (UINT)width;
	textureDesc.Height = (UINT)height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textureDesc, nullptr, &texture);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE TEXTURE 2D", "IMGUI VIEWPORT");
		return;
	}
	Print("SUCCEEDED TO CREATE TEXTURE 2D", "IMGUI VIEWPORT");

	//RENDER TARGET VIEW
	if (rtv)
	{
		rtv->Release();
		rtv = nullptr;
	}

	hr = Graphics::Inst().GetDevice().CreateRenderTargetView(texture, nullptr, &rtv);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE TEXTURE 2D", "IMGUI VIEWPORT");
		return;
	}

	//DEPTH STENCIL TEXTURE
	D3D11_TEXTURE2D_DESC dsTextureDesc = {};
	dsTextureDesc.Width = (UINT)width;
	dsTextureDesc.Height = (UINT)height;
	dsTextureDesc.MipLevels = 1;
	dsTextureDesc.ArraySize = 1;
	dsTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	dsTextureDesc.SampleDesc.Count = 1;
	dsTextureDesc.SampleDesc.Quality = 0;
	dsTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsTextureDesc.CPUAccessFlags = 0;
	dsTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* dsTexture = nullptr;
	hr = Graphics::Inst().GetDevice().CreateTexture2D(&dsTextureDesc, nullptr, &dsTexture);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE TEXTURE 2D", "IMGUI VIEWPORT");
		return;
	}
	Print("SUCCEEDED TO CREATE TEXTURE 2D", "IMGUI VIEWPORT");


	//DEPTH STENCIL VIEW
	if (dsv)
	{
		dsv->Release();
		dsv = nullptr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvDesc.Texture2D.MipSlice = 0;
	hr = Graphics::Inst().GetDevice().CreateDepthStencilView(dsTexture, &dsvDesc, &dsv);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE DEPTH STENCIL VIEW", "IMGUI VIEWPORT");
		return;
	}
	Print("SUCCEEDED TO CREATE DEPTH STENCIL VIEW", "IMGUI VIEWPORT");


	//SHADER RESOURCE VIEW
	if (srv)
	{
		srv->Release();
		srv = nullptr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = Graphics::Inst().GetDevice().CreateShaderResourceView(texture, &srvDesc, &srv);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE SHADER RESOURCE VIEW", "IMGUI VIEWPORT");
		return;
	}
	Print("SUCCEEDED TO CREATE SHADER RESOURCE VIEW", "IMGUI VIEWPORT");

	dsTexture->Release();
	texture->Release();

	Print("SUCCEEDED TO INITIALIZE IMGUI VIEWPORT");
	Print("=======================================");

	viewportPanel.SetResource(srv);

	if (camera)
		camera->SetPerspectiveMatrix(viewport.Width, viewport.Height);
}

Editor::~Editor()
{
	rtv->Release();
	dsv->Release();
	srv->Release();
}