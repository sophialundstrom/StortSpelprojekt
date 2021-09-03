#pragma once

#include "Singleton.h"
#include <d3d11.h>
#include "Print.h"
#include <string>

class Graphics : public Singleton<Graphics>
{
private:
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	D3D11_VIEWPORT viewport;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBuffer;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	ID3D11RasterizerState* noCullWireframeState;
	ID3D11RasterizerState* noCullState;
	ID3D11Debug* debug;

	HRESULT CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window);
	HRESULT CreateRenderTarget();
	HRESULT CreateDepthStencil(UINT windowWidth, UINT windowHeight);
	HRESULT CreateRasterizerStates();
	void CreateViewport(UINT windowWidth, UINT windowHeight);
public:
	Graphics(UINT windowWidth, UINT windowHeight, HWND window);
	~Graphics();

	void BeginFrame();
	void EndFrame() { swapChain->Present(0, 0); }
	void EnableWireframe() { context->RSSetState(noCullWireframeState); }
	void DisableWireframe() { context->RSSetState(noCullState); }

	ID3D11Device& GetDevice() { return *device; };
	ID3D11DeviceContext& GetContext() { return *context; };
	ID3D11DepthStencilView& GetDSV() { return  *dsView; }
	D3D11_VIEWPORT& GetViewport() { return  viewport; }
	const float* GetBackgroundColor() { return backgroundColor; }
};