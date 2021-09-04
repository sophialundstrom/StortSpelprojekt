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
		window.AddButtonComponent("PARTICLE EDITOR", 200, 100);
	}

	void Update()
	{
		if (firstFrame)
			return;

		if (window.GetValue<ButtonComponent>("PLAY DEBUG MODE"))
			state = DB_PLAY;

		else if (window.GetValue<ButtonComponent>("LEVEL EDITOR"))
			state = DB_LEVEL;

		else if (window.GetValue<ButtonComponent>("PARTICLE EDITOR"))
			state = DB_PARTICLE;
	}

	void Render()
	{
		if (firstFrame)
			firstFrame = false;

		Graphics::Inst().BeginFrame();
		ImGUI::BeginFrame();

		window.Render();

		ImGUI::EndFrame();
		Graphics::Inst().EndFrame();
	}

	AppState GetState() { return state; }

	void Reset() { state = DB_MAIN; firstFrame = true; }
};