#include "UI.h"

UI::UI()
	:UIFactory(NULL), UIRenderTarget(NULL), lightSlateGrayBrush(NULL), cornflowerBlueBrush(NULL)
{

}

UI::~UI()
{
	SafeRelease(&UIFactory);
	SafeRelease(&UIRenderTarget);
	SafeRelease(&lightSlateGrayBrush);
	delete testButton;
}

HRESULT UI::Initialize(HWND window)
{
	HRESULT hr;
	hr = CreateDeviceIndependentResources();
	hr = CreateDeviceResources(window);

	UIwindow = window;

	testButton = new Button(buttonPos, 50, 50, cornflowerBlueBrush);

	return hr;
}

HRESULT UI::CreateDeviceIndependentResources()
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &UIFactory);

	return hr;
}

//ADD COLORED BRUSHES HERE
HRESULT UI::CreateDeviceResources(HWND window)
{
	HRESULT hr;

	if (!UIRenderTarget)
	{
		UINT dpiX;
		UINT dpiY;
		dpiX = dpiY = GetDpiForWindow(window);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dpiX,
				dpiY
			);
		RECT rc;
		GetClientRect(window, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		//Creates render target based on window(?)
		hr = UIFactory->CreateDxgiSurfaceRenderTarget(Graphics::Inst().GetSurface(), props, &UIRenderTarget);

		//GRAY BRUSH
		if (SUCCEEDED(hr))
		{
			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &lightSlateGrayBrush);
		}

		//BLUE BRUSH
		if (SUCCEEDED(hr))
		{
			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &cornflowerBlueBrush);
		}

		//RED BRUSH
		if(SUCCEEDED(hr))
		{
			hr = UIRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &crimsonBrush);
		}
	}
	return hr;
}

void UI::DiscardDeviceResources()
{
	SafeRelease(&UIRenderTarget);
	SafeRelease(&lightSlateGrayBrush);
	SafeRelease(&cornflowerBlueBrush);
}

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
	UIRenderTarget->FillRectangle(&rectangle1, lightSlateGrayBrush);
	//Outlined rectangle
	UIRenderTarget->DrawRectangle(&rectangle2, cornflowerBlueBrush);

	testButton->DrawButton(UIRenderTarget);

	if (Event::LeftIsClicked())
	{
		GetCursorPos(&mousePos);
		ScreenToClient(UIwindow, &mousePos);
		if(testButton->isClicked(mousePos.x, mousePos.y))
		{
			testButton->setBrush(crimsonBrush);
		}
	}
	else
	{
		testButton->setBrush(cornflowerBlueBrush);
	}
	

	//std::cout << Event::MousePosition().x << "	" << Event::MousePosition().y << std::endl;

	UIRenderTarget->EndDraw();
}
