#include "Text.h"
Text::Text()
{
	textStr = L"Default";
	bounds = { 0.0f,0.0f,50.0f,50.0f };
	fontSize = 10;
	static const WCHAR fontName[] = L"Verdana";
}

Text::~Text()
{
	textFormat->Release();
}

Text::Text(IDWriteFactory* writeFactory, std::wstring text, D2D_VECTOR_2F position, float width, float height, float fontSize)
{
	static const WCHAR fontName[] = L"Verdana";
	HRESULT hr;

	hr = writeFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);
	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	textStr = text;

	bounds = D2D1::RectF(position.x - (width / 2),
		position.y - (height / 2),
		position.x + (width / 2),
		position.y + (height / 2));
}

void Text::Draw(ID2D1RenderTarget* UIRenderTarget, ID2D1SolidColorBrush* brush)
{
	UIRenderTarget->DrawTextW(textStr.c_str(), (UINT32)textStr.size(), textFormat, bounds, brush);
}
