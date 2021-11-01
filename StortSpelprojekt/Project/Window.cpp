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
		Print((int)wParam);
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
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
{
	const wchar_t* className = L"Window Class";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	hWnd = CreateWindowEx(0, className, title,
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

	ActivateCursor();
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	this->width = clientRect.right;
	this->height = clientRect.bottom;
}

void Window::ActivateCursor()
{
	cursorEnabled = true;
	ShowCursor(true);
	Event::DisableMovement();
}

void Window::DeactivateCursor()
{
	cursorEnabled = false;
	ShowCursor(false);
	Event::EnableMovement();
}

void Window::ToggleCursor()
{
	if (cursorEnabled)
		DeactivateCursor();

	else
		ActivateCursor();
}