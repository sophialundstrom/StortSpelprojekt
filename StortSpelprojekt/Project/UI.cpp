#include "UI.h"

UI::UI(HWND window)
	:Singleton(this), UIwindow(window),
	UIFactory(nullptr), UIRenderTarget(nullptr), writeFactory(nullptr)
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &UIFactory);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE FACTORY","UI");
		return;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if FAILED(hr)
	{
		Print("FAILED TO CREATE WRITE FACTORY", "UI");
		return;
	}

	UINT dpiX, dpiY;
	dpiX = dpiY = GetDpiForWindow(window);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			(FLOAT)dpiX,
			(FLOAT)dpiY
		);

	RECT rc;
	GetClientRect(window, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	hr = UIFactory->CreateDxgiSurfaceRenderTarget(Graphics::Inst().GetSurface(), props, &UIRenderTarget);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE DXGI SURFACE RENDER TARGET", "UI");
		return;
	}

	for (UINT i = 0; i < ARRAYSIZE(brushColors); ++i)
	{
		Vector3 color = brushColors[i] * (1.0f / 255.0f);
		ID2D1SolidColorBrush* brush = nullptr;
		hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z), &brush);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE BRUSH", "UI");
			return;
		}

		brushes[(COLOR)i] = brush;
	}

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

	if (imageFactory)
		imageFactory->Release();
	if (UIFactory)
		UIFactory->Release();
	if (UIRenderTarget)
		UIRenderTarget->Release();
	if (writeFactory)
		writeFactory->Release();

	for (auto& [color, brush] : brushes)
		brush->Release();

	for (auto& [name, format] : textFormats)
		format->Release();
}