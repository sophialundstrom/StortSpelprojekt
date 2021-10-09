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

	//Creates render target based on window(?)
	hr = UIFactory->CreateDxgiSurfaceRenderTarget(Graphics::Inst().GetSurface(), props, &UIRenderTarget);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE DXGI SURFACE RENDER TARGET", "UI");
		return;
	}

	for (auto& color : brushColors)
	{
		ID2D1SolidColorBrush* brush = nullptr;
		hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF((UINT)color), &brush);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE BRUSH", "UI");
			return;
		}
		brushes[color] = brush;
	}

	for (auto& desc : textFormatDescs)
	{
		IDWriteTextFormat* format = nullptr;
		hr = writeFactory->CreateTextFormat(desc.fontName, desc.collection, desc.fontWeight, desc.fontStyle, desc.fontStretch, desc.fontSize, desc.localeName, &format);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE TEXT FORMAT", "UI");
			return;
		}
		textFormats[desc.format] = format;
	}

	Print("SUCCEEDED TO INITIALIZE UI");
}

UI::~UI()
{
	UIFactory->Release();
	UIRenderTarget->Release();
	writeFactory->Release();
	//lightSlateGrayBrush->Release();
	//cornflowerBlueBrush->Release();
	//crimsonBrush->Release();

	for (auto& [color, brush] : brushes)
		brush->Release();

	for (auto& [name, format] : textFormats)
		format->Release();

	//Not working?
	//SafeRelease(&UIFactory);
	//SafeRelease(&UIRenderTarget);
	//SafeRelease(&lightSlateGrayBrush);
	//SafeRelease(&cornflowerBlueBrush);
	//SafeRelease(&crimsonBrush);

	//delete testButton;
	//delete testImage;
	//delete testImage2;
	//delete testText;
}

//HRESULT UI::Initialize(HWND window)
//{
//	
//	//testButton = new Button(buttonPos, 50, 50, cornflowerBlueBrush);
//	//testImage = new Image(L"./Images/Pepsi.jpeg", UIRenderTarget, imagePos, 0.25f, 1.0f);
//	//testImage2 = new Image(L"./Images/Pepsi3.jpg", UIRenderTarget, image2Pos, 0.05f, 1.0f);
//	//testText = new Text(writeFactory, L"I'm a text!", image2Pos, 50, 30, 20);
//
//	//return hr;
//}

//HRESULT UI::CreateDeviceIndependentResources()
//{
//	HRESULT hr;
//
//	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &UIFactory);
//
//	return hr;
//}

//ADD COLORED BRUSHES HERE
//HRESULT UI::CreateDeviceResources(HWND window)
//{
//	HRESULT hr;
//
//	if (!UIRenderTarget)
//	{
//		UINT dpiX;
//		UINT dpiY;
//		dpiX = dpiY = GetDpiForWindow(window);
//
//		D2D1_RENDER_TARGET_PROPERTIES props =
//			D2D1::RenderTargetProperties(
//				D2D1_RENDER_TARGET_TYPE_DEFAULT,
//				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
//				(FLOAT)dpiX,
//				(FLOAT)dpiY
//			);
//		RECT rc;
//		GetClientRect(window, &rc);
//
//		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
//
//		//Creates render target based on window(?)
//		hr = UIFactory->CreateDxgiSurfaceRenderTarget(Graphics::Inst().GetSurface(), props, &UIRenderTarget);
//
//		//GRAY BRUSH
//		if (SUCCEEDED(hr))
//		{
//			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &lightSlateGrayBrush);
//		}
//
//		//BLUE BRUSH
//		if (SUCCEEDED(hr))
//		{
//			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &cornflowerBlueBrush);
//		}
//
//		//RED BRUSH
//		if(SUCCEEDED(hr))
//		{
//			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &crimsonBrush);
//		}
//	}
//	return hr;
//}

//DO UI STUFF HERE
void UI::Render()
{
	UIRenderTarget->BeginDraw();

	UIRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	D2D1_SIZE_F rtSize = UIRenderTarget->GetSize();

	//Drawing a grid background
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	//Draw 2 rectangles
	D2D1_RECT_F rectangle1 = D2D1::RectF(20.f, 20.f, rtSize.width / 8, rtSize.height - 20.f);
	D2D1_RECT_F rectangle2 = D2D1::RectF(1, 1, rtSize.width / 8 + 20.0f, rtSize.height);

	//Filled rectangle
	//UIRenderTarget->FillRectangle(&rectangle1, lightSlateGrayBrush);

	//Outlined rectangle
	//UIRenderTarget->DrawRectangle(&rectangle2, cornflowerBlueBrush);

	//testButton->DrawButton(UIRenderTarget);
	//testImage->DrawImage(UIRenderTarget);
	//testImage2->DrawImage(UIRenderTarget);
	//testText->Draw(UIRenderTarget, lightSlateGrayBrush);

	//if (Event::LeftIsClicked())
	//{
	//	GetCursorPos(&mousePos);
	//	ScreenToClient(UIwindow, &mousePos);
	//	if(testButton->isClicked(mousePos.x, mousePos.y))
	//	{
	//		testButton->setBrush(crimsonBrush);
	//	}
	//}
	//else
	//{
	//	testButton->setBrush(cornflowerBlueBrush);
	//}

	UIRenderTarget->EndDraw();
}
