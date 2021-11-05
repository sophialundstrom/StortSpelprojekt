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
		DARK_YELLOW,
		BLACK
	};

	enum class TEXTFORMAT
	{
		DEFAULT,
		TITLE_CENTERED,
		TITLE,
		TITLE_BACKGROUND,
		TITLE_SMALL
	};
private:
	//BRUSH COLORS
	const Vector3 brushColors[4]
	{
		{ 173, 174, 164 },
		{ 211, 184, 96 },
		{ 136, 116, 52 },
		{ 0, 0, 0 }

	};

	//TEXT FORMATS
	struct TextFormatDesc
	{
		TEXTFORMAT format = TEXTFORMAT::DEFAULT;
		const WCHAR* fontName				= L"Bahnschrift";
		IDWriteFontCollection* collection	= nullptr;
		DWRITE_FONT_WEIGHT fontWeight		= DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE fontStyle			= DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch		= DWRITE_FONT_STRETCH_NORMAL;
		FLOAT fontSize						= 15;
		DWRITE_TEXT_ALIGNMENT alignment		= DWRITE_TEXT_ALIGNMENT_LEADING;
		const WCHAR* localeName				= L"";
	};

	const TextFormatDesc textFormatDescs[5] =
	{
		TextFormatDesc(),
		{ TEXTFORMAT::TITLE, L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30 },
		{ TEXTFORMAT::TITLE_BACKGROUND, L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_EXTRA_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30 ,DWRITE_TEXT_ALIGNMENT_CENTER},
		{ TEXTFORMAT::TITLE_SMALL, L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20 },
		{ TEXTFORMAT::TITLE_CENTERED, L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_ULTRA_BLACK, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, DWRITE_TEXT_ALIGNMENT_CENTER }
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