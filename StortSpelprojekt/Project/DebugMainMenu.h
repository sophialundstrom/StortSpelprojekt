#pragma once
#include "ImGuiWin.h"
#include "GameState.h"
#include "StateENUM.h"

class DebugMainMenu : public GameState
{
private:
	bool firstFrame = true;
	ImGuiWin window;
private:
	void Render()
	{
		Graphics::Inst().BeginFrame();
		ImGUI::BeginFrame();

		window.Render();

		ImGUI::EndFrame();
		Graphics::Inst().EndFrame();
	}

public:
	DebugMainMenu(UINT clientWidth, UINT clientHeight)
		:window("DEBUG MAIN MENU")
	{
		scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 100.0f, 1.0f, 2.0f);
		scene.SetDirectionalLight(40);

		window.AddButtonComponent("PLAY DEBUG MODE", 200, 100);
		window.AddButtonComponent("LEVEL EDITOR", 200, 100);
		window.AddButtonComponent("PARTICLE EDITOR", 200, 100);
		window.AddButtonComponent("EXIT", 200, 100);
		(void)Run();
	}

	// Inherited via GameState
	State Run()
	{
		Render();

		if (window.GetValue<ButtonComponent>("PLAY DEBUG MODE"))
			return State::GAME;

		else if (window.GetValue<ButtonComponent>("LEVEL EDITOR"))
			return State::LEVEL;

		else if (window.GetValue<ButtonComponent>("PARTICLE EDITOR"))
			return State::PARTICLE;

		else if (window.GetValue<ButtonComponent>("EXIT"))
			return State::EXIT;

		return State::NO_CHANGE;
	}
};