#pragma once
#include "Editor.h"

class DebugMainMenu : public Editor
{
private:
	bool setup = false;
public:
	DebugMainMenu()
		:Editor("DEBUG MAIN MENU")
	{
		AddButtonComponent("PLAY DEBUG MODE", 200, 100);
		AddButtonComponent("LEVEL EDITOR", 200, 100);
		AddButtonComponent("TERRAIN EDITOR", 200, 100);
		AddButtonComponent("MATERIAL EDITOR", 200, 100);
	}

	void Update()
	{
		if (setup)
		{

		}
	}
};