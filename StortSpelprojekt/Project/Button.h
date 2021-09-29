#pragma once
#include <d2d1.h>
#include <d2d1helper.h>

class Button
{
private:
	D2D1_RECT_F bounds;
	ID2D1SolidColorBrush* buttonBrush;

public:
	Button();
	~Button();
	Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush);
	void setBrush(ID2D1SolidColorBrush* newBrush);

	bool isClicked(int xPos, int yPos);
	void DrawButton(ID2D1RenderTarget* UIRenderTarget);
};