#include "Button.h"
#include "Canvas.h"

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
}

Button::Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush, std::function<void()> onClickFunction, std::function<void()> onHoverFunction, bool visible)
	:onClickFunction(onClickFunction), onHoverFunction(onHoverFunction), visible(visible)
{
	bounds.left = pos.x - (width / 2);
	bounds.top = pos.y - (height / 2);
	bounds.right = pos.x + (width / 2);
	bounds.bottom = pos.y + (height / 2);

	buttonBrush = brush;
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

bool Button::IsHovered(int x, int y)
{
	if (x > bounds.left && x < bounds.right && y < bounds.bottom && y > bounds.top)
	{
		return true;
	}
	return false;
}

//Not needed..?
//Mouse coordinates as input
bool Button::isClicked(int xPos, int yPos)
{
	if (xPos > bounds.left && xPos < bounds.right && yPos < bounds.bottom && yPos > bounds.top)
	{
		return true;
	}
	return false;
}

void Button::Draw()
{
	UI::Inst().GetRenderTarget()->DrawRectangle(&bounds, buttonBrush);
}