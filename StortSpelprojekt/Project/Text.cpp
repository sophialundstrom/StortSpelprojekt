#include "Text.h"
#include "UI.h"

void Text::SetWidth()
{
	width = format->GetFontSize() * string.length();
}

Text::Text(std::wstring string, D2D_VECTOR_2F position, UI::TEXTFORMAT format, ID2D1SolidColorBrush* brush, bool visible)
	:string(string), brush(brush)
{
	this->format = UI::Inst().GetTextFormat(format);
	this->visible = visible;
	this->height = this->format->GetFontSize(); 
	this->backgroundFormat = UI::Inst().GetTextFormat((UI::TEXTFORMAT)((UINT)format-1));
	this->backgroundBrush = UI::Inst().GetBrush(UI::COLOR::BROWN);
	SetWidth();

	if (this->format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
	{
		bounds = D2D1::RectF(position.x,
			position.y,
			position.x + width,
			position.y + width);
	}

	else
	{
		bounds = D2D1::RectF(position.x - (width / 2),
			position.y - (height / 2),
			position.x + (width / 2),
			position.y + (height / 2));
	}
}

Text::Text(std::wstring string, D2D_VECTOR_2F position, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush, FLOAT width, FLOAT height, bool visible)
	:string(string), format(format), brush(brush), UIComponent(width, height, visible)
{
	bounds = D2D1::RectF(position.x,
		position.y,
		position.x + width,
		position.y + width);
}

void Text::SetString(const std::string newString, bool bound)
{
	this->string = to_wstr(newString);

	if (format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
	{
		const auto lastPosition = GetLeftSidePosition();
		if (!bound)
			SetWidth();
		SetLeftSidePosition((int)lastPosition.x, (int)lastPosition.y);
	}
		
	else
	{
		const auto lastPosition = GetPosition();
		if (!bound)
			SetWidth();
		SetPosition((int)lastPosition.x, (int)lastPosition.y);
	}
		
}

void Text::Draw(bool allCharacters, UINT numCharacters)
{


	if (allCharacters)
	{
		UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), (UINT32)string.size(), backgroundFormat, bounds, backgroundBrush);
		UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), (UINT32)string.size(), format, bounds, brush);

	}
	else
	{
		UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), numCharacters, backgroundFormat, bounds, backgroundBrush);

		UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), numCharacters, format, bounds, brush);

	}

}
