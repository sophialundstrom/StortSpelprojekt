#include "Win.h"

void hoveringWin()
{
	Print("Hovering");
}

void hovering2Win()
{
	Print("Hovering");
}

Win::Win(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, true),
	particleRenderer(FORWARD)
{
	currentCanvas = new Canvas();

	//FORM
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 1.25f }, "Form", "Form.png", 1.f, true);

	//QUIT
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 1.6f }, "Quit", 400, 150, UI::COLOR::GRAY, [this] { QuitGame(); }, hoveringWin);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 1.6f }, "Quit", "Quit.png", 1.f, true);

	//MAINMENU
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 2.2f }, "Name", 400, 150, UI::COLOR::GRAY, [this] { MainMenu(); }, hovering2Win);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 2.2f }, "MainMenuButton", "MainMenuButton.png", 1.f, true);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 5.5f }, "Win", "Win.png", 1.f, true);

	(void)Run();
}

Win::~Win()
{
	for (auto& [name, canvas] : canvases)
		delete canvas;
}


void Win::MainMenu()
{
	backToMenu = true;
}

void Win::QuitGame()
{
	quit = true;
}

void Win::Render()
{
	Graphics::Inst().BeginFrame();
	currentCanvas->Render();
	Graphics::Inst().EndFrame();
}

APPSTATE Win::Run()
{

	currentCanvas->Update();
	Render();


	if (Event::KeyIsPressed('M'))
		gameStateStatus = APPSTATE::MAIN_MENU;

	if (quit)
		return APPSTATE::EXIT;

	if (backToMenu)
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		gameStateStatus = APPSTATE::EXIT;

	return gameStateStatus;
}
