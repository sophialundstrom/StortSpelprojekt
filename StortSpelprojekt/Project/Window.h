#pragma once
#include <Windows.h>
#include <vector>

class Window
{
	friend struct WindowCreator;
private:
	static std::vector<char> rawBuffer;

	static bool wantExit;

	static UINT width, height;
	static HWND hWnd;

	static bool cursorEnabled;
	
	static LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void ShutDown() { DestroyWindow(hWnd); }

	static void Resize(UINT newWidth = 0, UINT newHeight = 0);

	static POINT GetMousePosition();

	static void ToggleCursor();
	static void ActivateCursor();
	static void DeactivateCursor();

	static bool CursorIsActive() { return cursorEnabled; }

	static UINT ClientWidth()	{ return width; }
	static UINT ClientHeight()	{ return height; }

	static bool Exit()			{ return wantExit; }

	static HWND GetHWND()		{ return hWnd; }
};
inline std::vector<char> Window::rawBuffer;
inline bool Window::wantExit;
inline UINT Window::width;
inline UINT Window::height;
inline HWND Window::hWnd;
inline bool Window::cursorEnabled = true;

struct WindowCreator
{
	WindowCreator(Window window, UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
};