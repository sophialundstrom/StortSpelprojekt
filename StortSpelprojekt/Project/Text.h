#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <string>
#include <dwrite.h>

class Text
{
private:
	std::wstring textStr;
	D2D1_RECT_F bounds;
	IDWriteTextFormat* textFormat;
	float fontSize;
public:
	Text();
	~Text();
	Text(IDWriteFactory* writeFactory, std::wstring text, D2D_VECTOR_2F position, float width, float height, float fontSize);

	void Draw(ID2D1RenderTarget* UIRenderTarget, ID2D1SolidColorBrush* brush);
};