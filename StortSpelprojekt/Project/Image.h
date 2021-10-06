#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>

class Image
{
private:
	D2D1_RECT_F bounds;
	ID2D1Bitmap* bitMap;
	IWICImagingFactory* imageFactory;
	IWICBitmapDecoder* decoder;
	IWICBitmapFrameDecode* source;
	IWICFormatConverter* converter;
	float scale = 1;
	float opacity = 1;
	float width;
	float height;

public:
	Image();
	~Image();
	Image(LPCWSTR filename, ID2D1RenderTarget* UIRenderTarget, D2D_VECTOR_2F position, float scale = 1.0f, float opacity = 1.0f);

	void DrawImage(ID2D1RenderTarget* UIRenderTarget);
};