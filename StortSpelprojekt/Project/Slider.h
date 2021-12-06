#pragma once
#include "Button.h"
#include "Image.h"
#include "UIComponent.h"

class Slider : public UIComponent
{
private:
	Button* button;
	Image* sliderImage;
	Image* buttonImage;

	D2D_VECTOR_2F position;

	float currentValue;
	float minValue;
	float maxValue;

	float minX;
	float maxX;

	std::function<void(float)> func;
public:

	Slider(D2D_VECTOR_2F position, Button* button, Image* sliderImage, Image* buttonImage, float minValue, float maxValue, float currentValue, std::function<void(float)> func = NULL, bool visible = true);
	Slider() = default;
	~Slider();

	void Draw();

	void Update();
};