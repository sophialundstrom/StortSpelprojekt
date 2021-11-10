#include "UI.h"

#include "Window.h"

UI::UI()
	:Singleton(this)
{
	HRESULT hr;

	//FACTORY
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE FACTORY","UI");
		return;
	}

	//WRITE FACTORY
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if FAILED(hr)
	{
		Print("FAILED TO CREATE WRITE FACTORY", "UI");
		return;
	}

	//RENDER TARGET
	UINT dpiX, dpiY;
	dpiX = dpiY = GetDpiForWindow(Window::GetHWND());
	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			(FLOAT)dpiX,
			(FLOAT)dpiY
		);

	RECT rc;
	GetClientRect(Window::GetHWND(), &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	hr = factory->CreateDxgiSurfaceRenderTarget(Graphics::Inst().GetSurface(), props, &renderTarget);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE DXGI SURFACE RENDER TARGET", "UI");
		return;
	}

	//BRUSHES
	for (UINT i = 0; i < ARRAYSIZE(brushColors); ++i)
	{
		Vector3 color = brushColors[i] * (1.0f / 255.0f);
		ID2D1SolidColorBrush* brush = nullptr;
		hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z), &brush);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE BRUSH", "UI");
			return;
		}

		brushes[(COLOR)i] = brush;
	}

	//TEXT FORMATS
	for (auto& desc : textFormatDescs)
	{
		IDWriteTextFormat* format = nullptr;
		hr = writeFactory->CreateTextFormat(desc.fontName, nullptr, desc.fontWeight, desc.fontStyle, desc.fontStretch, desc.fontSize, desc.localeName, &format);
		
		if FAILED(hr)
		{
			Print("FAILED TO CREATE TEXT FORMAT", "UI");
			return;
		}

		format->SetTextAlignment(desc.alignment);
		textFormats[desc.format] = format;
	}

	//IMAGE FACTORY
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&imageFactory);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE IMAGE FACTORY", "UI");
		return;
	}

	Print("SUCCEEDED TO INITIALIZE UI");
}

UI::~UI()
{
	imageFactory->Release();
	factory->Release();
	renderTarget->Release();
	writeFactory->Release();

	for (auto& [color, brush] : brushes)
		brush->Release();

	for (auto& [name, format] : textFormats)
		format->Release();
}

ID2D1SolidColorBrush* UI::GetBrush(COLOR color)
{
	if (brushes.find(color) != brushes.end())
		return brushes[color];
	else
		return brushes.begin()->second;
}

IDWriteTextFormat* UI::GetTextFormat(TEXTFORMAT format)
{
	if (textFormats.find(format) != textFormats.end())
		return textFormats[format];
	else
		return textFormats.begin()->second;
}