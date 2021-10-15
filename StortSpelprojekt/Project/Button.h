#pragma once
#include "UIComponent.h"
#include <d2d1helper.h>
#include <functional>

class Button : public UIComponent
{
private:
	ID2D1SolidColorBrush* buttonBrush;
	std::function<void()> onClickFunction; 
	std::function<void()> onHoverFunction;
public:
	Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush, std::function<void()> onClickFunction = NULL, std::function<void()> onHoverFunction = NULL, bool visible = true);
	void setBrush(ID2D1SolidColorBrush* newBrush);

	bool IsHovered(int x, int y);
	bool isClicked(int xPos, int yPos);
	void OnClickFunction() { if (onClickFunction == NULL) return; onClickFunction(); }
	void OnHoverFunction() { if (onHoverFunction == NULL) return; onHoverFunction(); }
	void Draw();
};