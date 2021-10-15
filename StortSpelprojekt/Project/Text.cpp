#include "Text.h"
#include "UI.h"

Text::Text(std::wstring string, D2D_VECTOR_2F position, float width, float height, IDWriteTextFormat* format, ID2D1SolidColorBrush* brush, bool visible)
	:string(string), format(format), brush(brush), UIComponent(width, height, visible)
{
	//textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	bounds = D2D1::RectF(position.x - (width / 2),
		position.y - (height / 2),
		position.x + (width / 2),
		position.y + (height / 2));
}

void Text::Draw()
{
	UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), (UINT32)string.size(), format, bounds, brush);
}
