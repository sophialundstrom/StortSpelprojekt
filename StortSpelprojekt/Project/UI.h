#pragma once
#include "Math.h"
#include <dwrite.h>
#include <wincodec.h>
#include "Graphics.h"
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
		BLACK,
		BROWN
	};

	enum class TEXTFORMAT
	{
		DEFAULT_BACKGROUND,
		DEFAULT,
		TITLE_BACKGROUND,
		TITLE_CENTERED,
		TITLE__SMALL_BACKGROUND,
		TITLE_SMALL,

	};
private:
	//BRUSH COLORS
	const Vector3 brushColors[5]
	{
		{ 173, 174, 164 },
		{ 211, 184, 96 },
		{ 136, 116, 52 },
		{ 0, 0, 0 },
		{ 43, 17, 1 }
	};
	//TEXT FORMATS
	struct TextFormatDesc
	{
		TEXTFORMAT format = TEXTFORMAT::DEFAULT;
		const WCHAR* fontName				= L"Lato";
		IDWriteFontCollection* collection	= nullptr;
		DWRITE_FONT_WEIGHT fontWeight		= DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE fontStyle			= DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch		= DWRITE_FONT_STRETCH_NORMAL;
		FLOAT fontSize						= 15;
		DWRITE_TEXT_ALIGNMENT alignment		= DWRITE_TEXT_ALIGNMENT_LEADING;
		const WCHAR* localeName				= L"";
	};

	const TextFormatDesc textFormatDescs[6] =
	{
		TextFormatDesc(),
		{ TEXTFORMAT::DEFAULT_BACKGROUND, L"Lato", nullptr, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, DWRITE_TEXT_ALIGNMENT_LEADING},
		{ TEXTFORMAT::TITLE_BACKGROUND, L"Lato", nullptr, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30 , DWRITE_TEXT_ALIGNMENT_CENTER},
		{ TEXTFORMAT::TITLE_CENTERED, L"Lato", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, DWRITE_TEXT_ALIGNMENT_CENTER },
		{ TEXTFORMAT::TITLE__SMALL_BACKGROUND, L"Lato", nullptr, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, DWRITE_TEXT_ALIGNMENT_LEADING},
		{ TEXTFORMAT::TITLE_SMALL, L"Lato", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, DWRITE_TEXT_ALIGNMENT_LEADING },
	};
	
	IDWriteFontCollection* fontCollection = nullptr;
	ID2D1Factory* factory = nullptr;
	ID2D1RenderTarget* renderTarget = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	IWICImagingFactory* imageFactory = nullptr;
	
	std::map<TEXTFORMAT, IDWriteTextFormat*> textFormats;
	std::map<COLOR, ID2D1SolidColorBrush*> brushes;
public:
	UI();
	~UI();

	void BeginFrame()	{ renderTarget->BeginDraw(); renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());}
	void EndFrame()		{ renderTarget->EndDraw(); }

	ID2D1SolidColorBrush* GetBrush(COLOR color);
	IDWriteTextFormat* GetTextFormat(TEXTFORMAT format);

	IWICImagingFactory* GetImageFactory()	{ return imageFactory; }
	IDWriteFactory* GetWriteFactory()		{ return writeFactory; }
	ID2D1RenderTarget* GetRenderTarget()	{ return renderTarget; }
};