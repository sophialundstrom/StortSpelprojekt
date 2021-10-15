#pragma once
#include <d2d1.h>

class UIComponent
{
protected:
	bool visible;
	D2D1_RECT_F bounds = {};
	float width, height;
public:
	UIComponent(float width, float height, bool visible = true)
		:visible(visible), width(width), height(height) {}

	void SetPosition(int x, int y)
	{
		bounds.left = x - (width / 2);
		bounds.right = x + (width / 2);
		bounds.top = y + (height / 2);
		bounds.bottom = y - (height / 2);
	}

	bool IsVisible() { return visible; }
	bool Hide() { visible = false; }
	bool Show() { visible = true; }
	bool ToggleVisibility() { (visible == true) ? visible = false : visible = true; }
};