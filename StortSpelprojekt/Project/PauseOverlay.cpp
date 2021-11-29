#include "PauseOverlay.h"
#include "Time.h"
#include "Event.h"

void PauseOverlay::HideLeaves()
{
	GetImage("ResumeLeaves")->Hide();
	GetImage("MainMenuLeaves")->Hide();
	GetImage("OptionsLeaves")->Hide();
	GetImage("HowToPlayLeaves")->Hide();
}

PauseOverlay::PauseOverlay()
{

	//X-COORD FOR ROW
	float x = 75.0f;

	//LEFT BORDER WOOD
	AddImage({ 25, Window::ClientHeight() / 2.0f }, "MenuStick", "MenuStick.png", 0.8f, 1.0f, true, true);
	AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f }, "ABPauseBackground", "PauseBackground.png", 1.0f, 1.0f);


	{
		auto resumeFunc = [this]() { returnState = OVERLAYSTATE::RETURN; };
		auto hoveringFunc = [this]() { GetImage("ResumeLeaves")->Show(); };
		AddImage({ x, Window::ClientHeight() / 2.0f - 75 }, "Resume", "Resume.png", 0.8f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f - 75 }, "ResumeLeaves", "ResumeLeaves.png", 0.8f, 1.0f, true, false);
		auto image = GetImage("Resume");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "ResumeButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, resumeFunc, hoveringFunc);
	}

	{
		auto toMainMenuChoiceFunc = [this]() { internalState = INTERNALSTATE::QUIT_CHOICE; };
		auto hoveringFunc = [this]() { GetImage("MainMenuLeaves")->Show(); };
		AddImage({ x, Window::ClientHeight() / 2.0f }, "MainMenu", "MainMenu.png", 0.8f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f }, "MainMenuLeaves", "MainMenuLeaves.png", 0.8f, 1.0f, true, false);
		auto image = GetImage("MainMenu");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "MainMenuButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, toMainMenuChoiceFunc, hoveringFunc);
	}

	{
		// HOW TO PLAY
		auto toStateFunc = [this]() { internalState = INTERNALSTATE::HOW_TO_PLAY; };
		auto hoveringFunc = [this]() { GetImage("HowToPlayLeaves")->Show(); };

		AddImage({ x, Window::ClientHeight() / 2.0f + 75 }, "HowToPlay", "HowToPlay.png", 0.8f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f + 75 }, "HowToPlayLeaves", "HowToPlayLeaves.png", 0.8f, 1.0f, true, false);
		auto image = GetImage("HowToPlay");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "HowToPlayButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, toStateFunc, hoveringFunc);
	}

	{
		// OPTIONS
		auto toOptionsChoiceFunc = [this]() {internalState = INTERNALSTATE::OPTIONS; };
		auto hoveringFunc = [this]() { GetImage("OptionsLeaves")->Show(); };

		AddImage({ x, Window::ClientHeight() / 2.0f + 150 }, "Options", "Options.png", 0.8f, 1.0f, true, false);
		AddImage({ x, Window::ClientHeight() / 2.0f + 150 }, "OptionsLeaves", "OptionsLeaves.png", 0.8f, 1.0f, true, false);
		auto image = GetImage("Options");
		AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "OptionsButton", image->GetWidth(), image->GetHeight(), UI::COLOR::DARK_YELLOW, toOptionsChoiceFunc, hoveringFunc);
	}

	auto htpCanvas = new Canvas();
	{
		float xPos = Window::ClientWidth() / 2.0f + 250.f;
		float yPos = Window::ClientHeight() / 2.0f + 100.0f;

		htpCanvas->AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 8.0f }, "HTPTitle", "HowToPlaySmall.png", 1.f, 1.0f);
		htpCanvas->AddImage({ std::roundf(xPos), std::roundf(yPos) }, "Controls", "ControlsBig.png", 1.f, 1.0f);
		

	}
	canvases["HTP"] = htpCanvas;

	auto pauseTitleCanvas = new Canvas();
	{
		pauseTitleCanvas->AddImage({ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 8.0f }, "PauseTitle", "PAUSED.png", 0.8f, 1.0f);

	}
	canvases["PAUSETITLE"] = pauseTitleCanvas;

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
	
	HideLeaves();

	if (showCursor)
		currentCursor->Draw();

	UI::Inst().EndFrame();
}

OVERLAYSTATE PauseOverlay::Update()
{
	returnState = OVERLAYSTATE::NO_CHANGE;

	Canvas::Update();

	if (Event::KeyIsPressed(VK_ESCAPE) && Time::Get() - Event::LastKeyPress(VK_ESCAPE) > 2.f)
	{
		returnState = OVERLAYSTATE::RETURN;
		lastInput = Time::Get();
	}

	if (internalState == INTERNALSTATE::HOW_TO_PLAY)
	{
		currentCanvas = canvases["HTP"];
		GetImage("HowToPlayLeaves")->Show();
	}

	if (internalState == INTERNALSTATE::MAIN)
	{
		currentCanvas = canvases["PAUSETITLE"];
	}



	return returnState;
}