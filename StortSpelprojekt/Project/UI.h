#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"

//Function to release interface
template<class Interface>
inline void SafeRelease(Interface** interfaceToRelease)
{
	if (*interfaceToRelease == NULL)
	{
		(*interfaceToRelease)->Release();

		(*interfaceToRelease) = 0;
	}
}

//Macros for error handling
#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)){OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif
#endif Assert

//Module's base address
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class UI
{
private:
	ID2D1Factory* UIFactory;
	ID2D1RenderTarget* UIRenderTarget;
	ID2D1SolidColorBrush* lightSlateGrayBrush;
	ID2D1SolidColorBrush* cornflowerBlueBrush;
public:
	UI();
	~UI();

	HRESULT Initialize(HWND window);
	void RunMessageLoop(); //Does this already exist? And is there a way to access it?
	void Render();
	ID2D1RenderTarget& GetUIRenderTarget() { return *UIRenderTarget; }

private:
	//Device independent resources, unless device is easily accessible
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND window);
	void DiscardDeviceResources();
	
	void OnResize(UINT width, UINT height);
};