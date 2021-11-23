#pragma once
#include "Canvas.h"

enum class OVERLAYSTATE { NO_CHANGE, PAUSE, MAIN_MENU, RETURN };

class Overlay : public Canvas
{
protected:
	OVERLAYSTATE returnState = OVERLAYSTATE::NO_CHANGE;
public:
	virtual void Render() = 0;
	virtual OVERLAYSTATE Update() = 0;
};