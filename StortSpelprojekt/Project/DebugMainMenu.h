#pragma once
#include "ImGuiWin.h"
#include "StateDefines.h"

class DebugMainMenu
{
private:
	bool firstFrame = true;
	AppState state = DB_MAIN;
	ImGuiWin window;
public:
	DebugMainMenu()
		:window("DEBUG MAIN MENU")
	{
		window.AddButtonComponent("PLAY DEBUG MODE", 200, 100);
		window.AddButtonComponent("LEVEL EDITOR", 200, 100);
		window.AddButtonComponent("TERRAIN EDITOR", 200, 100);
	}

	void Update()
	{
		if (firstFrame)
			return;

		if (window.GetValue<ButtonComponent>("PLAY DEBUG MODE"))
			state = DB_PLAY;

		else if (window.GetValue<ButtonComponent>("LEVEL EDITOR"))
			state = DB_LEVEL;

		else if (window.GetValue<ButtonComponent>("TERRAIN EDITOR"))
			state = DB_TERRAIN;
	}

	void Render()
	{
		Graphics::BeginFrame();
		ImGUI::BeginFrame();

		window.Render();

		if (firstFrame)
			firstFrame = false;

		ImGUI::EndFrame();
		Graphics::EndFrame();
	}

	AppState GetState() { return state; }

	void Reset() { state = DB_MAIN; firstFrame = true; }
};