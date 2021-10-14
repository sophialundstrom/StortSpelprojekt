#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <functional>

class Button
{
private:
	bool visible;
	D2D1_RECT_F bounds;
	ID2D1SolidColorBrush* buttonBrush;
	std::function<void()> onClickFunction; 
	std::function<void()> onHoverFunction;
public:
	Button();
	~Button();
	Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush, std::function<void()> onClickFunction = NULL, std::function<void()> onHoverFunction = NULL, bool visible = true);
	Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush);
	void setBrush(ID2D1SolidColorBrush* newBrush);

	bool IsHovered(int x, int y);
	bool isClicked(int xPos, int yPos);
	void OnClickFunction() { if (onClickFunction == NULL) return; onClickFunction(); }
	void OnHoverFunction() { if (onHoverFunction == NULL) return; onHoverFunction(); }
	void Draw();

	bool IsVisible() { return visible; }
	bool Hide() { visible = false; }
	bool Show() { visible = true; }
	bool ToggleVisibility() { (visible == true) ? visible = false : visible = true; }
};