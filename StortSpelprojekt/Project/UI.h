#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include "Math.h"
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"
#include "Event.h"
#include <d2d1.h>
#include <map>

class UI : public Singleton<UI>
{
public:
	enum class COLOR
	{
		GRAY
	};

	enum class TEXTFORMAT
	{
		DEFAULT,
		TITLE,
		TITLE_SMALL
	};
private:
	//BRUSH COLORS
	const Vector3 brushColors[1]
	{
		{ 173, 174, 164 }
	};

	//TEXT FORMATS
	struct TextFormatDesc
	{
		TEXTFORMAT format = TEXTFORMAT::DEFAULT;
		const WCHAR* fontName				= L"Georgia";
		IDWriteFontCollection* collection	= nullptr;
		DWRITE_FONT_WEIGHT fontWeight		= DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE fontStyle			= DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch		= DWRITE_FONT_STRETCH_NORMAL;
		FLOAT fontSize						= 15;
		const WCHAR* localeName				= L"";

		//ALIGNTMENTS HERE ALSO
	};

	const TextFormatDesc textFormatDescs[3] =
	{
		TextFormatDesc(),
		{ TEXTFORMAT::TITLE, L"Georgia", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30 },
		{ TEXTFORMAT::TITLE_SMALL, L"Georgia", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20 }
	};

	ID2D1Factory* UIFactory = nullptr;
	ID2D1RenderTarget* UIRenderTarget = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	IWICImagingFactory* imageFactory = nullptr;
	std::map<TEXTFORMAT, IDWriteTextFormat*> textFormats;
	std::map<COLOR, ID2D1SolidColorBrush*> brushes;
	HWND UIwindow;
public:
	UI(HWND window);
	~UI();

	//HRESULT Initialize(HWND window);

	void Render();

	void BeginFrame() { UIRenderTarget->BeginDraw();  UIRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());}
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

	IWICImagingFactory* GetImageFactory() { return imageFactory; }
	IDWriteFactory* GetWriteFactory() { return writeFactory; }
	ID2D1RenderTarget* GetRenderTarget() { return UIRenderTarget; }
private:
	//Device independent resources, unless device is easily accessible
	//HRESULT CreateDeviceIndependentResources();
	//HRESULT CreateDeviceResources(HWND window);
};