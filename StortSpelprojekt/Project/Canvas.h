#pragma once
#include "UI.h"
#include "Button.h"
#include "Image.h"
#include "Text.h"

class Canvas
{
private:
	std::map<std::string, Button*> buttons;
	std::map<std::string, Image*> images;
	std::map<std::string, Text*> texts;
public:
	Canvas() = default;
	~Canvas()
	{
		for (auto& [name, button] : buttons)
			delete button;

		for (auto& [name, image] : images)
			delete image;

		for (auto& [name, text] : texts)
			delete text;
	}

	void Update()
	{
		POINT mp = UI::Inst().GetMousePosition();

		for (auto& [name, button] : buttons)
		{
			if (button->IsHovered(mp.x, mp.y))
			{
				button->OnHoverFunction();

				if (Event::LeftIsClicked())
					button->OnClickFunction();

				break;
			}
		}
	}

	void Render()
	{
		UI::Inst().BeginFrame();

		for (auto& [name, button] : buttons)
			if (button->IsVisible())
				button->Draw();

		for (auto& [name, image] : images)
			if (image->IsVisible())
				image->Draw();

		for (auto& [name, text] : texts)
			if (text->IsVisible())
				text->Draw();

		UI::Inst().EndFrame();
	}

	void UpdateText(const std::string name, const std::string newString) { texts[name]->SetString(newString); }

	void AddButton(D2D_VECTOR_2F position, const std::string name, UINT width, UINT height, UI::COLOR color, std::function<void()> onClickFunction = NULL, std::function<void()> onHoverFunction = NULL, bool visible = true) { buttons.emplace(name, new Button(position, width, height, UI::Inst().GetBrush(color), onClickFunction, onHoverFunction, visible)); }
	void AddImage(D2D_VECTOR_2F position, const std::string name, const std::string fileName, float scale = 1.0f, float opacity = 1.0f, bool visible = true) { images[name] = new Image(fileName, position, scale, opacity, visible); }
	void AddText(D2D_VECTOR_2F position, const std::string name, const std::string string, float width, float height, UI::COLOR color, UI::TEXTFORMAT format, bool visible = true) { texts.emplace(name, new Text(std::wstring(string.begin(), string.end()), position, width, height, UI::Inst().GetTextFormat(format), UI::Inst().GetBrush(color), visible)); }

	void RemoveButton(const std::string& name) { if (buttons.find(name) != buttons.end()) buttons.erase(name); }
	void RemoveImage(const std::string& name) { if (images.find(name) != images.end()) images.erase(name); }
	void RemoveText(const std::string& name) { if (texts.find(name) != texts.end()) texts.erase(name); }

	template <typename T>
	bool Exists(const std::string& name) 
	{ 
		if (std::is_same_v<T, Button>)
		{
			if (buttons.find(name) != buttons.end())
				return true;
		}

		else if (std::is_same_v<T, Image>)
		{
			if (images.find(name) != images.end())
				return true;
		}

		else
		{
			if (texts.find(name) != texts.end())
				return true;
		}

		return false;
	}
};