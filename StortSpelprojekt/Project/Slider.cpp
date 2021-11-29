#include "Slider.h"
#include "Event.h"
#include "Window.h"

Slider::Slider(D2D_VECTOR_2F position, Button * button, Image* sliderImage, Image* buttonImage, float minValue, float maxValue, float currentValue, std::function<void(const std::string&, float)> func = NULL)
	:button(button), sliderImage(sliderImage), buttonImage(buttonImage), minValue(minValue), maxValue(maxValue), currentValue(currentValue)
{
	sliderImage->SetPosition(position.x, position.y);

	minX = position.x - sliderImage->GetWidth() / 2;
	maxX = position.x + sliderImage->GetWidth() / 2;

	float startX =  minX + sliderImage->GetWidth() * ((currentValue - minValue) / (maxValue - minValue));

	buttonImage->SetPosition(startX, position.y);

	button->SetPosition(startX, position.y);
	
	


}

Slider::~Slider()
{
}

void Slider::Update()
{
	if (Event::LeftIsClicked())
	{
		POINT mp = Window::GetMousePosition();

		if (button->isClicked(mp.x, mp.y))
		{
			//auto lastPosition = button->GetPosition();
			if (mp.x > maxX)
			{
				mp.x = maxX;
			}
			else if (mp.x < minX)
			{
				mp.x = minX;
			}
			button->SetPosition(mp.x, sliderImage->GetPosition().y);
			buttonImage->SetPosition(mp.x, sliderImage->GetPosition().y);

			

		}
	}
		


}
