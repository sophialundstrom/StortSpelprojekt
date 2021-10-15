#pragma once
#include "UIComponent.h"
#include <d2d1helper.h>
#include <wincodec.h>
#include <string>

class Image : public UIComponent
{
private:
	ID2D1Bitmap* bitMap;
	float scale;
	float opacity;
	float sourceWidth;
	float sourceHeight;
public:
	Image() = default;
	Image(const std::string& filename, D2D_VECTOR_2F position, float scale = 1.0f, float opacity = 1.0f, bool visible = true);
	~Image();
	void Draw();
};