#pragma once
#include "UI.h"

class Button
{
private:
	RECT bounds;
	ID2D1Brush* buttonBrush;

public:
	Button();
	Button(D2D_VECTOR_2F pos, float width, float height, ID2D1SolidColorBrush* brush);

	bool isClicked(int xPos, int yPos);
	void DrawButton();
};