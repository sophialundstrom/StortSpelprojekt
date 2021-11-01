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
	//{ (float)clientWidth / 2.f, (float)clientHeight / 2.f }, "C", 250, 250, UI::COLOR::GRAY, [this] { Test(); }, TestFuncOptions
	//ui = std::make_unique<UI>(window);
	currentCanvas = new Canvas();
	//currentCanvas->AddImage({ (float)clientWidth /2.f, (float)clientHeight / 2.f }, "GameOver", "GameOver.png", 2.f, true);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 2.f }, "MainMenuButton", "MainMenuButton.png", 2.f, true);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 4.f }, "Quit", "Quit.png", 2.f, true);

	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 2.f }, "Name", 400, 150, UI::COLOR::GRAY, [this] { MainMenu(); }, hovering);
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 4.f }, "Quit", 400, 150, UI::COLOR::GRAY, [this] { QuitGame(); }, hovering2);

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
