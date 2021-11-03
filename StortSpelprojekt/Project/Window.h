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

	bool cursorEnabled = true;
	
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window() = default;
	Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	~Window() { DestroyWindow(hWnd); }

	void ActivateCursor();
	void DeactivateCursor();

	void ToggleCursor();
	bool CursorIsActive() { return this->cursorEnabled; }

	UINT ClientWidth() { return width; }
	UINT ClientHeight() { return height; }
	bool Exit() { return wantExit; }
	HWND GetHWND() { return hWnd; }
};