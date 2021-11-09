#pragma once
#include <Windows.h>
#include <vector>

class Window
{
	friend class WindowCreator;
private:
	static std::vector<char> rawBuffer;

	static bool wantExit;

	static UINT width, height;
	static HWND hWnd;

	static bool cursorEnabled;
	static bool isInitialized;
	
	static LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	//~Window() { DestroyWindow(hWnd); }

	static void ShutDown() { DestroyWindow(hWnd); isInitialized = false; }

	static void ActivateCursor();
	static void DeactivateCursor();

	static void Resize(UINT newWidth = 0, UINT newHeight = 0);

	static void ToggleCursor();
	static bool CursorIsActive() { return cursorEnabled; }

	static UINT ClientWidth()	{ return width; }
	static UINT ClientHeight()	{ return height; }
	static bool Exit()			{ return wantExit; }
	static HWND GetHWND()		{ return hWnd; }
	static bool IsInitialized() { return isInitialized; }
};

inline std::vector<char> Window::rawBuffer;
inline bool Window::wantExit;
inline UINT Window::width;
inline UINT Window::height;
inline HWND Window::hWnd;
inline bool Window::cursorEnabled = true;
inline bool Window::isInitialized;

struct WindowCreator
{
	void Initialize(Window& window, UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
	{
		window.width = width;
		window.height = height;

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

		ShowWindow(window.hWnd, SW_SHOWDEFAULT);

		window.isInitialized = true;
	}
};