#include "Canvas.h"

#include "Window.h"
#include "Event.h"

Canvas::Canvas()
{
	regularCursor = new Image("RegularCursor.png", { 0,0 });
	hoveredCursor = new Image("HoveredCursor.png", { 0,0 });
	currentCursor = regularCursor;
}

Canvas::~Canvas()
{
	delete hoveredCursor;
	delete regularCursor;

	for (auto& [name, button] : buttons)
		if (button)
		{
			delete button;
			button = nullptr;
		}

	for (auto& [name, image] : images)
		if (image)
		{
			delete image;
			image = nullptr;
		}

	for (auto& [name, text] : texts)
		if (text)
		{
			delete text;
			text = nullptr;
		}
}

void Canvas::Update()
{
	POINT mp = Window::GetMousePosition();

	if (showCursor)
	{
		currentCursor = regularCursor;
		hoveredCursor->SetPosition(mp.x, mp.y);
		regularCursor->SetPosition(mp.x, mp.y);
	}
	
	for (auto& [name, button] : buttons)
	{
		if (button->IsHovered(mp.x, mp.y))
		{
			currentCursor = hoveredCursor;
			button->OnHoverFunction();

			if (Event::LeftIsClicked())
				button->OnClickFunction();

			break;
		}
	}
}

void Canvas::DrawImages()
{
	for (auto& [name, image] : images)
		if (image->IsVisible())
			image->Draw();
}

void Canvas::DrawTexts()
{
	for (auto& [name, text] : texts)
		if (text->IsVisible())
			text->Draw();
}

void Canvas::DrawButtons()
{
	for (auto& [name, button] : buttons)
		if (button->IsVisible())
			button->Draw();
}

void Canvas::Render()
{
	UI::Inst().BeginFrame();

	DrawImages();

	//DrawButtons();

	DrawTexts();

	if (showCursor)
		currentCursor->Draw();

	UI::Inst().EndFrame();
}

Button* Canvas::GetButton(const std::string& name)
{
	if (buttons.find(name) != buttons.end())
		return buttons[name];
	return nullptr;
}

Image* Canvas::GetImage(const std::string& name)
{
	if (images.find(name) != images.end())
		return images[name];
	return nullptr;
}

Text* Canvas::GetText(const std::string& name)
{
	if (texts.find(name) != texts.end())
		return texts[name];
	return nullptr;
}