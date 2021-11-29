#pragma once
#include "UIComponent.h"
#include <d2d1helper.h>
#include <wincodec.h>
#include <string>
class Image : public UIComponent
{
private:
	ID2D1Bitmap* bitMap;
	std::string filename;
	float scale;
	float opacity;
	float sourceWidth;
	float sourceHeight;
public:
	Image() = default;
	Image(const std::string& filename, D2D_VECTOR_2F position, float scale = 1.0f, float opacity = 1.0f, bool visible = true, bool centered = true);
	~Image();
	float GetWidth() { return width; }
	float GetHeight() { return height; }
	void Draw();
	std::string FileName() { return filename; }
};