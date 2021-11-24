#include "PauseOverlay.h"

PauseOverlay::PauseOverlay()
{
	//AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f }, "PauseBackground", "PauseBackground.png", 1.0f, 1.0f);
	//AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 8.0f }, "PauseTitle", "PAUSED.png", 1.0f, 1.0f);
	//
	//AddButton({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f - 100 }, "RESUME", 350, 95, UI::COLOR::GRAY, [this] { Resume(); });
	//AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f - 100 }, "ResumeButton", "ResumeButton.png", 0.50f, 1.0f);
	//
	//AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f }, "HowToPlayButton", "HowToPlayButton.png", 0.50f, 1.0f);
	//AddButton({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f }, "HowToPlay", 350, 95, UI::COLOR::GRAY, [this] { HowToPlay(); });
	//
	//AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f + 100 }, "BackToMainMenu", "MainMenuButton.png", 0.50f, 1.0f);
	//AddButton({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f + 100 }, "BackToMainMenuButton", 350, 95, UI::COLOR::GRAY, [this] { MainMenu(); });

}

PauseOverlay::~PauseOverlay()
{
}

void PauseOverlay::Render()
{
}

OVERLAYSTATE PauseOverlay::Update()
{
	returnState = OVERLAYSTATE::NO_CHANGE;

	return returnState;
}