#pragma once

#include <d3d11.h>
#include "Print.h"
#include <string>

class Graphics
{
private:
	static constexpr float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	static D3D11_VIEWPORT viewport;
	static ID3D11Device* device;
	static ID3D11DeviceContext* context;
	static IDXGISwapChain* swapChain;
	static ID3D11RenderTargetView* backBuffer;
	static ID3D11Texture2D* dsTexture;
	static ID3D11DepthStencilView* dsView;
	static ID3D11RasterizerState* noCullWireframeState;
	static ID3D11RasterizerState* noCullState;
	static ID3D11Debug* debug;

	static HRESULT CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window);
	static HRESULT CreateRenderTarget();
	static HRESULT CreateDepthStencil(UINT windowWidth, UINT windowHeight);
	static HRESULT CreateRasterizerStates();
	static void CreateViewport(UINT windowWidth, UINT windowHeight);
public:
	static bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);
	static void BeginFrame();
	static void EndFrame() { swapChain->Present(0, 0); }
	static void EnableWireframe() { context->RSSetState(noCullWireframeState); }
	static void DisableWireframe() { context->RSSetState(noCullState); }
	static void ShutDown();

	static ID3D11Device& GetDevice() { return *device; };
	static ID3D11DeviceContext& GetContext() { return *context; };
	static ID3D11DepthStencilView& GetDSV() { return  *dsView; }
	static D3D11_VIEWPORT& GetViewport() { return  viewport; }
	static const float* GetBackgroundColor() { return backgroundColor; }
};

inline D3D11_VIEWPORT Graphics::viewport;
inline ID3D11Device* Graphics::device;
inline ID3D11DeviceContext* Graphics::context;
inline IDXGISwapChain* Graphics::swapChain;
inline ID3D11RenderTargetView* Graphics::backBuffer;
inline ID3D11Texture2D* Graphics::dsTexture;
inline ID3D11DepthStencilView* Graphics::dsView;
inline ID3D11RasterizerState* Graphics::noCullWireframeState;
inline ID3D11RasterizerState* Graphics::noCullState;
inline ID3D11Debug* Graphics::debug;