#pragma once

#include "Singleton.h"
#include <d3d11.h>
#include "Print.h"
#include <string>

class Graphics : public Singleton<Graphics>
{
private:
	const float backgroundColor[4] = { 0, 0, 0, 1.0f };

	D3D11_VIEWPORT viewport;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBuffer;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	IDXGISurface* UISurface;

	ID3D11BlendState* blendState = nullptr;
	ID3D11BlendState* blendStateATC = nullptr;

	ID3D11RasterizerState* wireframeState;

	HRESULT CreateBlendState();
	HRESULT CreateBlendStateATC();
	HRESULT CreateDeviceSwapchain(UINT clientWidth, UINT clientHeight, HWND hWnd, bool windowed);
	HRESULT CreateRenderTarget();
	HRESULT CreateDepthStencil(UINT clientWidth, UINT clientHeight);
	HRESULT CreateRasterizerState();
	void CreateViewport(UINT clientWidth, UINT clientHeight);
public:
	Graphics(UINT clientWidth, UINT clientHeight, HWND hWnd, bool windowed = true);
	~Graphics();

	void BeginFrame();
	void BeginFrame(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT viewport);
	void EndFrame() { swapChain->Present(0, 0); }

	void EnableAlpha() { Graphics::Inst().GetContext().OMSetBlendState(blendState, nullptr, 0Xffffffff); }
	void DisableAlpha() { Graphics::Inst().GetContext().OMSetBlendState(nullptr, nullptr, 0Xffffffff); }
	void EnableATCAlpha() { Graphics::Inst().GetContext().OMSetBlendState(blendStateATC, nullptr, 0Xffffffff); }

	void ActivateWireframe()	{ context->RSSetState(wireframeState); }
	void DeactivateWireframe()	{ context->RSSetState(nullptr); }
	void ToggleWireframe() 
	{ 
		ID3D11RasterizerState* currentState = nullptr; 
		context->RSGetState(&currentState); 
		
		if (!currentState) 
			context->RSSetState(wireframeState); 
		else 
			context->RSSetState(nullptr); 
	}

	ID3D11Device& GetDevice()			{ return *device; };
	ID3D11DeviceContext& GetContext()	{ return *context; };
	ID3D11DepthStencilView& GetDSV()	{ return  *dsView; }
	D3D11_VIEWPORT& GetViewport()		{ return  viewport; }
	const float* GetBackgroundColor()	{ return backgroundColor; }
	IDXGISurface* GetSurface()			{ return UISurface; }
};