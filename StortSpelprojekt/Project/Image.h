#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>
#include <string>

class Image
{
private:
	D2D1_RECT_F bounds;
	ID2D1Bitmap* bitMap;
	bool visible;
	float scale;
	float opacity;
	float width;
	float height;
	float sourceWidth;
	float sourceHeight;
public:
	Image() = default;
	Image(const std::string& filename, D2D_VECTOR_2F position, float scale = 1.0f, float opacity = 1.0f, bool visible = true);
	~Image();
	bool IsVisible() { return visible; }
	bool Hide() { visible = false; }
	bool Show() { visible = true; }
	bool ToggleVisibility() { (visible == true) ? visible = false : visible = true; }
	void Draw();
};