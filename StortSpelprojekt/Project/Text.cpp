#include "Text.h"
#include "UI.h"

void Text::SetWidth()
{
	width = format->GetFontSize() * string.length();
}

Text::Text(std::wstring string, D2D_VECTOR_2F position, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush, bool visible)
	:string(string), format(format), brush(brush), UIComponent(0, format->GetFontSize(), visible)
{
	SetWidth();
	if (format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
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

void Text::SetString(const std::string newString)
{
	this->string = to_wstr(newString);

	if (format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
	{
		const auto lastPosition = GetLeftSidePosition();
		SetWidth();
		SetLeftSidePosition((int)lastPosition.x, (int)lastPosition.y);
	}
		
	else
	{
		const auto lastPosition = GetPosition();
		SetWidth();
		SetPosition((int)lastPosition.x, (int)lastPosition.y);
	}
		
}

void Text::Draw()
{
	//UI::Inst().GetRenderTarget()->DrawRectangle(bounds, brush);
	UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), (UINT32)string.size(), format, bounds, brush);
}
