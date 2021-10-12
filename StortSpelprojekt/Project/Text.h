#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <string>
#include <dwrite.h>

class Text
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	IDWriteTextFormat* format = nullptr;
	std::wstring string;
	D2D1_RECT_F bounds;
public:
	Text() = default;
	Text(std::wstring string, D2D_VECTOR_2F position, float width, float height, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush);
	void SetString(const std::string newString) { this->string = std::wstring(newString.begin(), newString.end()); }
	std::string GetString() { return std::string(string.begin(), string.end()); }
	void Draw();
};