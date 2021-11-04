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
	:modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED)
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
}

GameOver::~GameOver()
{
	delete currentCanvas;
}

void GameOver::Initialize()
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());

	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			shadowRenderer.Bind(model);
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}

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
