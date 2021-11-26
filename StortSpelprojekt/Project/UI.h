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
		DEFAULT,
		TITLE_CENTERED,
		TITLE_SMALL,
		DIALOGUE
	};
private:
	static const std::wstring FONTFILE;
	static const std::wstring FONTNAME;

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
		const WCHAR* fontName				= FONTNAME.c_str();
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
		{ TEXTFORMAT::TITLE_CENTERED, FONTNAME.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, DWRITE_TEXT_ALIGNMENT_CENTER },
		{ TEXTFORMAT::TITLE_SMALL, FONTNAME.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, DWRITE_TEXT_ALIGNMENT_LEADING }
	};
	
	IDWriteFontCollection* fontCollection = nullptr;
	ID2D1Factory* factory = nullptr;
	ID2D1RenderTarget* renderTarget = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	IWICImagingFactory* imageFactory = nullptr;
	
	IDWriteFontFace* fontFace = nullptr;
	IDWriteFontFile* fontFile = nullptr;

	std::map<TEXTFORMAT, IDWriteTextFormat*> textFormats;
	std::map<COLOR, ID2D1SolidColorBrush*> brushes;
public:
	UI();
	~UI();

	void BeginFrame()	{ renderTarget->BeginDraw(); renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());}
	void EndFrame()		{ renderTarget->EndDraw(); }

	ID2D1SolidColorBrush* GetBrush(COLOR color);
	IDWriteTextFormat* GetTextFormat(TEXTFORMAT format);

	ID2D1Factory* GetFactory()				{ return factory; }
	IDWriteFontFace* GetFontFace()			{ return fontFace; }
	IWICImagingFactory* GetImageFactory()	{ return imageFactory; }
	IDWriteFactory* GetWriteFactory()		{ return writeFactory; }
	ID2D1RenderTarget* GetRenderTarget()	{ return renderTarget; }
};

inline const std::wstring UI::FONTNAME = L"Palatino Linotype";
inline const std::wstring UI::FONTFILE = L"pala.ttf";