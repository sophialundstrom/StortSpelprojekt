#pragma once
#include "Canvas.h"
#include "Window.h"

enum class OVERLAYSTATE { NO_CHANGE, PAUSE, MAIN_MENU, RETURN };

class Overlay : public Canvas
{
protected:
	D2D_VECTOR_2F center;
	OVERLAYSTATE returnState = OVERLAYSTATE::NO_CHANGE;
public:
	Overlay()
	{
		center = { Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f };
	}

	virtual void Render() = 0;
	virtual OVERLAYSTATE Update() = 0;
};