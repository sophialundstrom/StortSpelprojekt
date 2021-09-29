#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"
#include "Button.h"
#include "Event.h"

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
	ID2D1SolidColorBrush* crimsonBrush;
	HWND UIwindow;
	POINT mousePos;

	D2D_VECTOR_2F buttonPos = {50.f, 50.f};
	Button* testButton;

public:
	UI();
	~UI();

	HRESULT Initialize(HWND window);
	void Render();

private:
	//Device independent resources, unless device is easily accessible
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND window);
	void DiscardDeviceResources();
	
};