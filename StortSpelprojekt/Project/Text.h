#pragma once
#include "UIComponent.h"
#include <d2d1helper.h>
#include <string>
#include <dwrite.h>

class Text : public UIComponent
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	IDWriteTextFormat* format = nullptr;
	std::wstring string;
public:
	Text() = default;
	Text(std::wstring string, D2D_VECTOR_2F position, float width, float height, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush, bool visible = true);
	void SetString(const std::string newString) { this->string = std::wstring(newString.begin(), newString.end()); }
	std::string GetString() { return std::string(string.begin(), string.end()); }
	void Draw();
};