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
		GRAY,
		YELLOW,
		DARK_YELLOW
	};

	enum class TEXTFORMAT
	{
		DEFAULT,
		TITLE_CENTERED,
		TITLE,
		TITLE_SMALL
	};
private:
	//BRUSH COLORS
	const Vector3 brushColors[3]
	{
		{ 173, 174, 164 },
		{ 211, 184, 96 },
		{ 136, 116, 52 }
	};

	//TEXT FORMATS
	struct TextFormatDesc
	{
		TEXTFORMAT format = TEXTFORMAT::DEFAULT;
		const WCHAR* fontName				= L"HighLevel";
		IDWriteFontCollection* collection	= nullptr;
		DWRITE_FONT_WEIGHT fontWeight		= DWRITE_FONT_WEIGHT_SEMI_BOLD;
		DWRITE_FONT_STYLE fontStyle			= DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch		= DWRITE_FONT_STRETCH_SEMI_EXPANDED;
		FLOAT fontSize						= 15;
		DWRITE_TEXT_ALIGNMENT alignment		= DWRITE_TEXT_ALIGNMENT_LEADING;
		const WCHAR* localeName				= L"";
	};

	const TextFormatDesc textFormatDescs[4] =
	{
		TextFormatDesc(),
		{ TEXTFORMAT::TITLE, L"HighLevel", nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_SEMI_EXPANDED, 30 },
		{ TEXTFORMAT::TITLE_SMALL, L"HighLevel", nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20 },
		{ TEXTFORMAT::TITLE_CENTERED, L"HighLevel", nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_SEMI_EXPANDED, 30, DWRITE_TEXT_ALIGNMENT_CENTER }
	};

	IDWriteFontCollection* fontCollection = nullptr;
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

	void Render();
	void BeginFrame() { UIRenderTarget->BeginDraw(); UIRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());}
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
};