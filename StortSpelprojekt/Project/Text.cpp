#include "Text.h"
#include "UI.h"

void Text::SetWidth()
{
	width = format->GetFontSize() * string.length();
	if (format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
		width -= width / 4.0f;
}

FLOAT Text::ConvertPointSizeToDIP(FLOAT points)
{
	return (points / 72.0f) * 96.0f;
}

void Text::CalculateOutline()
{
	const UINT stringLength = string.length();

	UINT* points = new UINT[stringLength];
	UINT16* indices = new UINT16[stringLength];

	ZeroMemory(points, sizeof(UINT) * stringLength);
	ZeroMemory(indices, sizeof(UINT16) * stringLength);

	for (UINT i = 0; i < stringLength; ++i)
		points[i] = string.at(i);

	auto fontFace = UI::Inst().GetFontFace();
	auto factory = UI::Inst().GetFactory();

	HRESULT hr;
	
	hr = fontFace->GetGlyphIndicesW(points, stringLength, indices);
	if FAILED(hr)
	{
		Print("FAILED TO GET GLYPH INDICES", "TEXT");
		return;
	}

	hr = factory->CreatePathGeometry(&geometry);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE PATH GEOMETRY", "TEXT");
		return;
	}

	hr = geometry->Open((ID2D1GeometrySink**)&outline);
	if FAILED(hr)
	{
		Print("FAILED TO OPEN GEOMETRY SINK", "TEXT");
		return;
	}
	
	hr = fontFace->GetGlyphRunOutline(ConvertPointSizeToDIP(format->GetFontSize()), indices, NULL, NULL, stringLength, FALSE, FALSE, outline);
	if FAILED(hr)
	{
		Print("FAILED TO GET OUTLINE", "TEXT");
		return;
	}

	hr = outline->Close();
	if FAILED(hr)
	{
		Print("FAILED TO CLOSE GEOMETRY SINK", "TEXT");
		return;
	}

	if (points)
	{
		delete[] points;
		points = nullptr;
	}

	if (indices)
	{
		delete[] indices;
		indices = nullptr;
	}
}

Text::~Text()
{
	if (geometry)
	{
		geometry->Release();
		geometry = nullptr;
	}

	if (outline)
	{
		outline->Release();
		outline = nullptr;
	}
}

Text::Text(std::wstring string, D2D_VECTOR_2F position, UI::TEXTFORMAT format, ID2D1SolidColorBrush* brush, bool visible)
	:string(string), brush(brush)
{
	this->format = UI::Inst().GetTextFormat(format);
	this->visible = visible;
	this->height = this->format->GetFontSize(); 
	this->backgroundBrush = UI::Inst().GetBrush(UI::COLOR::DARK_YELLOW);
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

	CalculateOutline();
}

Text::Text(std::wstring string, D2D_VECTOR_2F position, UI::TEXTFORMAT format, ID2D1SolidColorBrush* brush, FLOAT width, FLOAT height, bool visible)
	:string(string), brush(brush), UIComponent(width, height, visible)
{
	this->format = UI::Inst().GetTextFormat(format);
	this->visible = visible;
	this->backgroundBrush = UI::Inst().GetBrush(UI::COLOR::DARK_YELLOW);

	bounds = D2D1::RectF(position.x,
		position.y,
		position.x + width,
		position.y + width);

	CalculateOutline();
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

	CalculateOutline();
}

void Text::Draw(bool allCharacters, UINT numCharacters)
{
	UI::Inst().GetRenderTarget()->DrawRectangle(bounds, brush);

	if (allCharacters)
	{
		if (format->GetTextAlignment() == DWRITE_TEXT_ALIGNMENT_LEADING)
			UI::Inst().GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Translation(GetLeftSidePosition().x, GetLeftSidePosition().y));
		else
			UI::Inst().GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Translation(GetPosition().x - (width / 4.0f), GetPosition().y + height / 2.0f));

		UI::Inst().GetRenderTarget()->DrawGeometry(geometry, backgroundBrush, 2.0f);
		UI::Inst().GetRenderTarget()->FillGeometry(geometry, brush);
		UI::Inst().GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	else
	{
		UI::Inst().GetRenderTarget()->DrawTextW(string.c_str(), numCharacters, format, bounds, brush);
	}
}
