#include "Graphics.h"

HRESULT Graphics::CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = false;

#ifdef _DEBUG
	swapChainDesc.Windowed = true;
#endif

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags = 0;

#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	return D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);
}

HRESULT Graphics::CreateRenderTarget()
{
	HRESULT hr;
	ID3D11Texture2D* buffer = nullptr;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&buffer));

	if FAILED(hr)
	{
		Print("FAILED TO GET BACK BUFFER");
		return hr;
	}

	//RENDER TARGET VIEW AT BACK BUFFER
	hr = device->CreateRenderTargetView(buffer, nullptr, &backBuffer);

	buffer->Release();

	return hr;
}

HRESULT Graphics::CreateDepthStencil(UINT windowWidth, UINT windowHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = windowWidth;
	textureDesc.Height = windowHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, nullptr, &dsTexture);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE 2D TEXTURE");
		return hr;
	}

	hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsView);
	return hr;
}

HRESULT Graphics::CreateRasterizerStates()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthBias = true;
	hr = device->CreateRasterizerState(&desc, &noCullWireframeState);

	if FAILED(hr)
	{
		Print("FAILED TO CREATE WIREFRAME RASTERIZER STATE");
		return hr;
	}

	desc.FillMode = D3D11_FILL_SOLID;
	hr = device->CreateRasterizerState(&desc, &noCullState);

	return hr;
}

void Graphics::CreateViewport(UINT windowWidth, UINT windowHeight)
{
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if FAILED(CreateDeviceSwapchain(windowWidth, windowHeight, window))
	{
		Print("FAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}

	if FAILED(CreateRenderTarget())
	{
		Print("FAILED TO CREATE RENDER TARGET");
		return false;
	}

	if FAILED(CreateDepthStencil(windowWidth, windowHeight))
	{
		Print("FAILED TO CREATE DEPTH STENCIL");
		return false;
	}

	if FAILED(CreateRasterizerStates())
	{
		Print("FAILED TO CREATE RASTERIZER STATES");
		return false;
	}

	CreateViewport(windowWidth, windowHeight);
	return true;
}

void Graphics::BeginFrame()
{
	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &backBuffer, dsView);
	context->ClearRenderTargetView(backBuffer, backgroundColor);
	context->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::ShutDown()
{
	noCullState->Release();
	noCullWireframeState->Release();
	dsView->Release();
	dsTexture->Release();
	backBuffer->Release();
	swapChain->Release();

	context->Release();
	device->Release();
}