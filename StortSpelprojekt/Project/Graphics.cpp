#include "Graphics.h"

Graphics::Graphics(UINT clientWidth, UINT clientHeight, HWND hWnd, bool windowed)
	:Singleton(this)
{
	if FAILED(CreateDeviceSwapchain(clientWidth, clientHeight, hWnd, windowed))
	{
		Print("FAILED TO CREATE DEVICE AND SWAP CHAIN", "GRAPHICS");
		return;
	}
	Print("SUCCEEDED TO CREATE DEVICE AND SWAP CHAIN", "GRAPHICS");

	if FAILED(CreateRenderTarget())
	{
		Print("FAILED TO CREATE RENDER TARGET", "GRAPHICS");
		return;
	}
	Print("SUCCEEDED TO CREATE RENDER TARGET", "GRAPHICS");

	if FAILED(CreateDepthStencil(clientWidth, clientHeight))
	{
		Print("FAILED TO CREATE DEPTH STENCIL", "GRAPHICS");
		return;
	}
	Print("SUCCEEDED TO CREATE DEPTH STENCIL", "GRAPHICS");

	if FAILED(CreateRasterizerState())
	{
		Print("FAILED TO CREATE WIREFRAME RASTERIZER STATE", "GRAPHICS");
		return;
	}
	Print("SUCCEEDED TO CREATE WIREFRAME RASTERIZER STATE", "GRAPHICS");

	CreateViewport(clientWidth, clientHeight);

	Print("SUCCEEDED TO INITIALIZE GRAPHICS");
	Print("=======================================");
}

Graphics::~Graphics()
{
	dsView->Release();
	dsTexture->Release();
	backBuffer->Release();
	swapChain->Release();
	context->Release();
	device->Release();
	UISurface->Release();
}

HRESULT Graphics::CreateDeviceSwapchain(UINT clientWidth, UINT clientHeight, HWND hWnd, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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

	hr = buffer->QueryInterface<IDXGISurface>(&UISurface);

	//RENDER TARGET VIEW AT BACK BUFFER
	hr = device->CreateRenderTargetView(buffer, nullptr, &backBuffer);

	buffer->Release();

	return hr;
}

HRESULT Graphics::CreateDepthStencil(UINT clientWidth, UINT clientHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	
	textureDesc.Width = clientWidth;
	textureDesc.Height = clientHeight;
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

HRESULT Graphics::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	return device->CreateRasterizerState(&desc, &wireframeState);
}

void Graphics::CreateViewport(UINT clientWidth, UINT clientHeight)
{
	viewport.Width = static_cast<float>(clientWidth);
	viewport.Height = static_cast<float>(clientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

void Graphics::BeginFrame()
{
	BeginFrame(backBuffer, dsView, viewport);
}

void Graphics::BeginFrame(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT viewport)
{
	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &rtv, dsv);
	context->ClearRenderTargetView(rtv, backgroundColor);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
}

