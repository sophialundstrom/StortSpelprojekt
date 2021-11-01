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
		bounds.top = y - (height / 2);
		bounds.right = x + (width / 2);
		bounds.bottom = y + (height / 2);
	}

	void SetLeftSidePosition(int x, int y)
	{
		bounds.left = (FLOAT)x;
		bounds.top = (FLOAT)y;
		bounds.right = x + width;
		bounds.bottom = y + height;
	}

	D2D_VECTOR_2F GetPosition()
	{
		return { bounds.right - (width / 2), bounds.bottom - (height / 2) };
	}

	D2D_VECTOR_2F GetLeftSidePosition()
	{
		return { bounds.left, bounds.top };
	}

	bool IsVisible() { return visible; }
	void Hide() { visible = false; }
	void Show() { visible = true; }
	void ToggleVisibility() { (visible == true) ? visible = false : visible = true; }
};