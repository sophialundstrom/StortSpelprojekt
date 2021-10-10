#pragma once
#include "UI.h"
#include "Button.h"
#include "Image.h"
#include "Text.h"

class Canvas
{
private:
	std::map<std::string, Button> buttons;
	std::map<std::string, Image*> images;
	std::map<std::string, Text> texts;
public:
	Canvas()
	{

	}

	~Canvas()
	{

	}

	void Update()
	{
		POINT mp = UI::Inst().GetMousePosition();

		for (auto& [name, button] : buttons)
		{
			if (button.IsHovered(mp.x, mp.y))
			{
				if (Event::LeftIsClicked())
					button.ExecuteFunction();

				break;
			}
		}
	}

	void Render()
	{
		UI::Inst().BeginFrame();

		for (auto& [name, button] : buttons)
			button.Draw();

		for (auto& [name, image] : images)
			image->Draw();

		for (auto& [name, text] : texts)
			text.Draw();

		UI::Inst().EndFrame();
	}

	void UpdateText(const std::string name, const std::string newString) { texts[name].SetString(newString); }
	void AddButton(D2D_VECTOR_2F position, const std::string name, UINT width, UINT height, UI::COLOR color, std::function<void()> function) { buttons.emplace(name, Button(position, width, height, UI::Inst().GetBrush(color), function)); }
	void AddImage(D2D_VECTOR_2F position, const std::string name, const std::string fileName, float scale = 1.0f, float opacity = 1.0f) { images[name] = new Image(fileName, position, scale, opacity); }
	void AddText(D2D_VECTOR_2F position, const std::string name, const std::string string, float width, float height, UI::COLOR color, UI::TEXTFORMAT format) { texts.emplace(name, Text(std::wstring(string.begin(), string.end()), position, width, height, UI::Inst().GetTextFormat(format), UI::Inst().GetBrush(color))); }
};