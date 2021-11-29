#include "GameOver.h"

void hovering()
{
	Print("Hovering");
}
void hovering2()
{
	Print("Quit");
}

GameOver::GameOver(UINT clientWidth, UINT clientHeight, HWND window)
{

	currentCanvas = new Canvas();
	//FORM
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 1.25f }, "Form", "Form.png", 1.f, true);

	//QUIT
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 1.6f }, "Quit", 400, 150, UI::COLOR::GRAY, [this] { QuitGame(); }, hovering2);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 1.6f }, "Quit", "Quit.png", 1.f, true);

	//MAINMENU
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 2.2f }, "Name", 400, 150, UI::COLOR::GRAY, [this] { MainMenu(); }, hovering);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 2.2f }, "MainMenuButton", "MainMenuButton.png", 1.f, true);

	//GAMEOVERTEXT
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 5.5f }, "GameOverText", "GameOverText.png", 2.f, true);

	(void)Run();

	BindShaders(nullptr, nullptr, nullptr, nullptr, nullptr);
}

GameOver::~GameOver()
{
	delete currentCanvas;
}

void GameOver::MainMenu()
{
	backToMenu = true;
}

void GameOver::QuitGame()
{
	quit = true;
}

void GameOver::Render()
{
	Graphics::Inst().BeginFrame();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();
}

APPSTATE GameOver::Run()
{
	/*scene.Update();*/

	currentCanvas->Update();
	Render();

	if (quit)
		return APPSTATE::EXIT;

	if (backToMenu)
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;;
}