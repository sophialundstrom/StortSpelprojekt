#include "Button.h"

Button::Button()
{
	bounds.left = 0.0f;
	bounds.top = 0.0f;
	bounds.right = 50.0f;
	bounds.bottom = 50.0f;

	buttonBrush = 0;
}

Button::~Button()
{
	buttonBrush->Release();
}

Button::Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush)
{
	bounds.left = pos.x - (width / 2);
	bounds.top = pos.y - (height / 2);
	bounds.right = pos.x + (width / 2);
	bounds.bottom = pos.y + (height / 2);

	buttonBrush = brush;
}

void Button::setBrush(ID2D1SolidColorBrush* newBrush)
{
	buttonBrush = newBrush;
}

//Mouse coordinates as input
bool Button::isClicked(int xPos, int yPos)
{
	if (xPos > bounds.left && xPos < bounds.right && yPos < bounds.bottom && yPos > bounds.top)
	{
		return true;
	}
	return false;
}

void Button::DrawButton(ID2D1RenderTarget* UIRenderTarget)
{
	UIRenderTarget->FillRectangle(&bounds, buttonBrush);
}