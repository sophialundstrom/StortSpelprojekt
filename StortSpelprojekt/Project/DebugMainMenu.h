#pragma once
#include "ImGuiWin.h"
#include "ApplicationState.h"

class DebugMainMenu : public ApplicationState
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
	APPSTATE Run()
	{
		Render();

		if (window.GetValue<ButtonComponent>("PLAY DEBUG MODE"))
			return APPSTATE::GAME;

		else if (window.GetValue<ButtonComponent>("LEVEL EDITOR"))
			return APPSTATE::LEVEL;

		else if (window.GetValue<ButtonComponent>("PARTICLE EDITOR"))
			return APPSTATE::PARTICLE;

		else if (window.GetValue<ButtonComponent>("EXIT"))
			return APPSTATE::EXIT;

		return APPSTATE::NO_CHANGE;
	}
};