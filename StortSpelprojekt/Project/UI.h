#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"
#include "Event.h"
#include <d2d1.h>
#include <map>

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

class UI : public Singleton<UI>
{
public:
	enum class COLOR
	{
		LIGHTSLATEGRAY = D2D1::ColorF::LightSlateGray,
		CORNFLOWERBLUE = D2D1::ColorF::CornflowerBlue,
		CRIMSON = D2D1::ColorF::Crimson,
		RED = D2D1::ColorF::Red
	};

	enum class TEXTFORMAT
	{
		DEFAULT, //Verdana, normal wright, normal font style, normal font stretch, 30 font size
		TEST
	};
private:
	//BRUSHES
	const COLOR brushColors[4] =
	{
		COLOR::LIGHTSLATEGRAY,
		COLOR::CORNFLOWERBLUE,
		COLOR::CRIMSON,
		COLOR::RED
	};

	//TEXT FORMATS
	struct TextFormatDesc
	{
		TEXTFORMAT format = TEXTFORMAT::DEFAULT;
		const WCHAR* fontName				= L"Verdana";
		IDWriteFontCollection* collection	= nullptr;
		DWRITE_FONT_WEIGHT fontWeight		= DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE fontStyle			= DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch		= DWRITE_FONT_STRETCH_NORMAL;
		FLOAT fontSize						= 15;
		const WCHAR* localeName				= L"";

		//ALIGNTMENTS HERE ALSO
	};

	const TextFormatDesc textFormatDescs[2] =
	{
		TextFormatDesc(),
		{ TEXTFORMAT::TEST, L"Verdana", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50 }
	};

	ID2D1Factory* UIFactory;
	ID2D1RenderTarget* UIRenderTarget;
	IDWriteFactory* writeFactory;
	//ID2D1SolidColorBrush* lightSlateGrayBrush;
	//ID2D1SolidColorBrush* cornflowerBlueBrush;	
	//ID2D1SolidColorBrush* crimsonBrush;
	
	std::map<TEXTFORMAT, IDWriteTextFormat*> textFormats;
	std::map<COLOR, ID2D1SolidColorBrush*> brushes;
	HWND UIwindow;

	//D2D_VECTOR_2F buttonPos = { 50.f,50.f };
	//Button* testButton;
	//D2D_VECTOR_2F imagePos = { 87.f,600.f };
	//Image* testImage;
	//D2D_VECTOR_2F image2Pos = { 90.f,200.f };
	//Image* testImage2;
	//D2D_VECTOR_2F textPos = { 300.f,300.f };
	//Text* testText;
public:
	UI(HWND window);
	~UI();

	//HRESULT Initialize(HWND window);

	void Render();

	void BeginFrame() { UIRenderTarget->BeginDraw(); }
	void EndFrame() { UIRenderTarget->EndDraw(); }

	ID2D1SolidColorBrush* GetBrush(COLOR color)
	{
		if (brushes.find(color) != brushes.end())
			return brushes[color];
		else
			return brushes.begin()->second;
	}

	IDWriteTextFormat* GetTextFormat(TEXTFORMAT format)
	{
		if (textFormats.find(format) != textFormats.end())
			return textFormats[format];
		else
			return textFormats.begin()->second;
	}

	POINT GetMousePosition()
	{
		POINT mp;
		GetCursorPos(&mp);
		ScreenToClient(UIwindow, &mp);
		return mp;
	}

	IDWriteFactory* GetWriteFactory() { return writeFactory; }
	ID2D1RenderTarget* GetRenderTarget() { return UIRenderTarget; }
private:
	//Device independent resources, unless device is easily accessible
	//HRESULT CreateDeviceIndependentResources();
	//HRESULT CreateDeviceResources(HWND window);
};