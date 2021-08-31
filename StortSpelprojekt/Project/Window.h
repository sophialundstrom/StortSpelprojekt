#pragma once
#include <Windows.h>
#include <vector>


class Window
{
private:
	std::vector<char> rawBuffer;

	bool wantExit = false;

	UINT width, height;
	HWND hWnd;

	bool cursorEnabled;
	void ActivateCursor();
	void DeactivateCursor();

	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	~Window() { DestroyWindow(hWnd); }

	void ToggleCursor();
	bool CursorIsActive() { return this->cursorEnabled; }

	UINT Width() { return width; }
	UINT Height() { return height; }
	bool Exit() { return wantExit; }
	HWND HWnd() { return hWnd; }
};