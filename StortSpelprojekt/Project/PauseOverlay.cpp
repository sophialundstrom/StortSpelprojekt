#include "PauseOverlay.h"
#include "Event.h"

void PauseOverlay::HideLeaves()
{
	
}

PauseOverlay::PauseOverlay()
{
	auto resumeFunc = [this]() { returnState = OVERLAYSTATE::RETURN; };
	auto toMainMenuChoiceFunc =	[this]() { internalState = INTERNALSTATE::QUIT_CHOICE; };

	//X-COORD FOR ROW
	float x = 75.0f;

	//LEFT BORDER WOOD
	AddImage({ 25, Window::ClientHeight() / 2.0f }, "MenuStick", "MenuStick.png", 0.8f, 1.0f, true, true);

	AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f }, "PauseBackground", "PauseBackground.png", 1.0f, 1.0f);
	AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 8.0f }, "PauseTitle", "PAUSED.png", 1.0f, 1.0f);
	
	AddButton({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f - 100 }, "RESUME", 350, 95, UI::COLOR::GRAY, resumeFunc);
	AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f - 100 }, "ResumeButton", "ResumeButton.png", 0.50f, 1.0f);
	
	AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f + 100 }, "BackToMainMenu", "MainMenuButton.png", 0.50f, 1.0f);
	AddButton({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f + 100 }, "BackToMainMenuButton", 350, 95, UI::COLOR::GRAY, toMainMenuChoiceFunc);

	{
		// HOW TO PLAY
		auto toStateFunc = [this]() { internalState = INTERNALSTATE::HOW_TO_PLAY; };
		auto hoveringFunc = [this]() { GetImage("HowToPlayLeaves")->Show(); };

		AddImage({ x, Window::ClientHeight() / 2.0f + 75 }, "HowToPlay", "HowToPlay.png", 1.0f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f + 75 }, "HowToPlayLeaves", "HowToPlayLeaves.png", 1.0f, 1.0f, true, false);
		auto image = GetImage("HowToPlay");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "HowToPlayButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, toStateFunc, hoveringFunc);
	}

	{
		// OPTIONS
		auto toStateFunc = [this]() { internalState = INTERNALSTATE::OPTIONS; };
		auto hoveringFunc = [this]() { GetImage("OptionsLeaves")->Show(); };

		AddImage({ x, Window::ClientHeight() / 2.0f + 150 }, "Options", "Options.png", 1.0f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f + 150 }, "OptionsLeaves", "OptionsLeaves.png", 1.0f, 1.0f, true, false);
		auto image = GetImage("Options");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "OptionsButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, toStateFunc, hoveringFunc);
	}
}

PauseOverlay::~PauseOverlay()
{
}

void PauseOverlay::Render()
{
	UI::Inst().BeginFrame();

	DrawButtons();
	DrawImages();
	DrawTexts();

	if (currentCanvas)
	{
		currentCanvas->DrawButtons();
		currentCanvas->DrawImages();
		currentCanvas->DrawTexts();
	}

	if (showCursor)
		currentCursor->Draw();

	UI::Inst().EndFrame();
}

OVERLAYSTATE PauseOverlay::Update()
{
	returnState = OVERLAYSTATE::NO_CHANGE;

	Canvas::Update();

	if (Event::KeyIsPressed(VK_ESCAPE))
		returnState = OVERLAYSTATE::RETURN;

	return returnState;
}