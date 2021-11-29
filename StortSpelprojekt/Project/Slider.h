#pragma once
#include "Button.h"
#include "Image.h"

class Slider
{
private:

	Button* button;
	Image* sliderImage;
	Image* buttonImage;

	float currentValue;
	float minValue;
	float maxValue;

	float minX;
	float maxX;

	std::function<void(float)> func;

public:

	Slider(D2D_VECTOR_2F position, Button* button, Image* sliderImage, Image* buttonImage, float minValue, float maxValue, float currentValue, std::function<void(const std::string&, float)> func = NULL);
	Slider() = default;
	~Slider();

	void Update();
};