#include "Slider.h"
#include "Event.h"
#include "Window.h"

Slider::Slider(D2D_VECTOR_2F position, Button * button, Image* sliderImage, Image* buttonImage, float minValue, float maxValue, float currentValue, std::function<void(float)> func, bool visible)
	:position(position), button(button), sliderImage(sliderImage), buttonImage(buttonImage), minValue(minValue), maxValue(maxValue), currentValue(currentValue), func(func), UIComponent(0,0,visible)
{

	sliderImage->SetPosition((int)position.x, (int)position.y);

	minX = position.x - sliderImage->GetWidth() / 2;
	maxX = position.x + sliderImage->GetWidth() / 2;

	float startX =  minX + sliderImage->GetWidth() * ((currentValue - minValue) / (maxValue - minValue));

	buttonImage->SetPosition((int)startX, (int)position.y + 4.0f);

	button->SetPosition((int)startX, (int)position.y + 4.0f);
	
	func(currentValue);
}

Slider::~Slider()
{
}

void Slider::Draw()
{
	sliderImage->Draw();
	buttonImage->Draw();

	button->Draw();
}

void Slider::Update()
{
	if (Event::LeftIsClicked())
	{
		POINT mp = Window::GetMousePosition();

		if (button->isClicked(mp.x, mp.y))
		{

			if (mp.x > maxX)
			{
				mp.x = (long)maxX;
			}

			else if (mp.x < minX)
			{
				mp.x = (long)minX;
			}
			button->SetPosition((int)mp.x, (int)sliderImage->GetPosition().y + 4.0f);
			buttonImage->SetPosition((int)mp.x, (int)sliderImage->GetPosition().y + 4.0f);

			float normalizedValue = ((mp.x - minX) / (maxX - minX));
			func(normalizedValue);

		}
	}
		
}
