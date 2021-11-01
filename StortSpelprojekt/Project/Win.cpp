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
	:modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED)
{
	currentCanvas = new Canvas();
	//QUIT
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 1.4f }, "Quit", 400, 150, UI::COLOR::GRAY, [this] { QuitGame(); }, hoveringWin);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 1.4f }, "Quit", "Quit.png", 1.f, true);

	//MAINMENU
	currentCanvas->AddButton({ (float)clientWidth / 2.f, (float)clientHeight / 2.f }, "Name", 400, 150, UI::COLOR::GRAY, [this] { MainMenu(); }, hovering2Win);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 2.f }, "MainMenuButton", "MainMenuButton.png", 1.f, true);
	currentCanvas->AddImage({ (float)clientWidth / 2.f, (float)clientHeight / 4.f }, "Win", "Win.png", 1.f, true);

	(void)Run();
}

Win::~Win()
{
	delete currentCanvas;
}

void Win::Initialize()
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
