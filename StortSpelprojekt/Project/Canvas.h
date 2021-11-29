#pragma once
#include "Button.h"
#include "Image.h"
#include "Text.h"

class Canvas
{
protected:
	bool showCursor = true;

	Image* hoveredCursor;
	Image* regularCursor;
	Image* currentCursor;

	std::map<std::string, Button*> buttons;
	std::map<std::string, Image*> images;
	std::map<std::string, Text*> texts;
public:
	Canvas();
	~Canvas();

	virtual void Update();

	void DrawImages();
	void DrawTexts();
	void DrawButtons();

	virtual void Render();

	void HideCursor() { showCursor = false; }
	void ShowCursor() { showCursor = true; }

	void UpdateText(const std::string name, const std::string newString) { texts[name]->SetString(newString); }

	void AddButton(D2D_VECTOR_2F position, const std::string name, float width, float height, UI::COLOR color, std::function<void()> onClickFunction = NULL, std::function<void()> onHoverFunction = NULL, bool visible = true) 
	{ buttons.emplace(name, new Button(position, width, height, UI::Inst().GetBrush(color), onClickFunction, onHoverFunction, visible)); }
	
	void AddImage(D2D_VECTOR_2F position, const std::string name, const std::string fileName, float scale = 1.0f, float opacity = 1.0f, bool visible = true, bool centered = true) 
	{ images[name] = new Image(fileName, position, scale, opacity, visible, centered); }
	
	void AddText(D2D_VECTOR_2F position, const std::string name, const std::string string, UI::COLOR color, UI::TEXTFORMAT format, bool visible = true) 
	{ texts.emplace(name, new Text(to_wstr(string), position, format, UI::Inst().GetBrush(color), visible)); }

	void RemoveButton(const std::string& name)	{ if (buttons.find(name) != buttons.end()) buttons.erase(name); }
	void RemoveImage(const std::string& name)	{ if (images.find(name) != images.end()) images.erase(name); }
	void RemoveText(const std::string& name)	{ if (texts.find(name) != texts.end()) texts.erase(name); }

	Button* GetButton(const std::string& name);
	Image* GetImage(const std::string& name);
	Text* GetText(const std::string& name);

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
	};
};