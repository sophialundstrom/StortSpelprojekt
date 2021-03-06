#include "Window.h"

#include "Event.h"
#include "ImGUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		wnd = reinterpret_cast<Window*>(cs->lpCreateParams);

		SetLastError(0);

		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd)))
			if (GetLastError() != 0)
				return FALSE;
	}

	else
		return wnd->MessageHandler(hWnd, msg, wParam, lParam);

	if (wnd)
		return wnd->MessageHandler(hWnd, msg, wParam, lParam);

	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Event::ResetScroll();

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_CLOSE:
		{
			wantExit = true;
			break;
		}
		case WM_KEYDOWN:
		{
			Event::OnKeyPressed(static_cast<unsigned int>(wParam));
			break;
		}

		case WM_KEYUP:
		{
			Event::OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}

		case WM_MOUSEWHEEL:
		{
			Event::Scrolled(static_cast<int>(wParam));
			break;
		}

		case WM_LBUTTONDOWN:
		{
			POINT pt;
			if (GetCursorPos(&pt))
				Event::OnLeftClick(pt.x, pt.y);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			POINT pt;
			if (GetCursorPos(&pt))
				Event::OnRightClick(pt.x, pt.y);
			break;
		}

		case WM_LBUTTONUP:
		{
			Event::OnLeftRelease();
			break;
		}

		case WM_RBUTTONUP:
		{
			Event::OnRightRelease();
			break;
		}

		case WM_INPUT:
		{
			UINT size;

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
				break;

			rawBuffer.resize(size);

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
				break;

			auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
			{
				Event::OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			}

			break;
		}

		case WM_SETFOCUS:
		{
			DeactivateCursor();
			break;
		}

		case WM_KILLFOCUS:
		{
			ActivateCursor();
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

POINT Window::GetMousePosition()
{
	POINT mp;
	GetCursorPos(&mp);
	ScreenToClient(hWnd, &mp);
	return mp;
}

void Window::ActivateCursor()
{
	ClipCursor(nullptr);
	cursorEnabled = true;
	ShowCursor(true);
}

void Window::DeactivateCursor()
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
	cursorEnabled = false;
	ShowCursor(false);
}

void Window::ToggleCursor()
{
	if (cursorEnabled)
		DeactivateCursor();

	else
		ActivateCursor();
}

void Window::Resize(UINT newWidth, UINT newHeight)
{
	if (newWidth == 0)
	{
		newWidth = GetSystemMetrics(SM_CXSCREEN);
		newHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	width = newWidth;
	height = newHeight;

	SetWindowPos(hWnd, nullptr, (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2), width, height, 0);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	width = clientRect.right;
	height = clientRect.bottom;
}

WindowCreator::WindowCreator(Window window, UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
{
	const wchar_t* className = L"Window Class";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = window.WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	window.hWnd = CreateWindowEx(0, className, title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, this);

	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));

	#ifdef _DEBUG
		AllocConsole();
		(void)freopen("conout$", "w", stdout);
	#endif

	RECT clientRect;
	GetClientRect(window.hWnd, &clientRect);
	window.width = clientRect.right;
	window.height = clientRect.bottom;

	ShowWindow(window.hWnd, SW_SHOWDEFAULT);
}