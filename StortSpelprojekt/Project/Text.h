#pragma once
#include "UIComponent.h"

inline std::string to_str(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

inline std::wstring to_wstr(const std::string& str)
{
	int count = (int)MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &wstr[0], count);
	return wstr;
}

class Text : public UIComponent
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	ID2D1SolidColorBrush* backgroundBrush = nullptr;
	IDWriteTextFormat* format = nullptr;
	
	ID2D1PathGeometry* geometry = nullptr;
	ID2D1GeometrySink* outline = nullptr;

	std::wstring string;
	void SetWidth();

	FLOAT ConvertPointSizeToDIP(FLOAT points);
	void CalculateOutline();
public:
	Text() = default;
	~Text();
	Text(std::wstring string, D2D_VECTOR_2F position, UI::TEXTFORMAT format, ID2D1SolidColorBrush* brush, bool visible = true);
	Text(std::wstring string, D2D_VECTOR_2F position, UI::TEXTFORMAT format, ID2D1SolidColorBrush* brush, FLOAT width, FLOAT height, bool visible = true);
	void SetString(const std::string newString, bool bound = false);
	std::string GetString() { return to_str(string); }
	FLOAT GetExactWidth();
	void Draw(bool allCharacters = true, UINT numCharacters = 0);
};