#include "Main Menu.h"

void MainMenu::Options()
{
	currentCanvas = canvases["OPTIONS"];
}

void MainMenu::HowToPlay()
{

	currentCanvas = canvases["HOW TO PLAY"];
}

void MainMenu::BacktoMenu()
{
	currentCanvas = canvases["MAIN MENU"];
}

void MainMenu::Play()
{
	play = true;
}

void MainMenu::Quit()
{
	quit = true;
}

void MainMenu::Form()
{
	ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW);
}

void Hovering()
{
	Print("HOVERING");
}

MainMenu::MainMenu(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD, 40)
{
	Initialize();

	auto menuCanvas = new Canvas();
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 5.0f }, "K", "ArcusLogo.png", 0.5f, 1.0f);

	menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.10f }, "O", 360, 100, UI::COLOR::GRAY, [this] { Play(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.10f }, "L", "PlayButton.png", 0.6f, 1.0f);
	
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.7f }, "G", "HowToPlayButton.png", 0.6f, 1.0f);
	menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.7f }, "A", 360, 100, UI::COLOR::GRAY, [this] { HowToPlay(); }, Hovering);

	menuCanvas->AddButton({ clientWidth / 8.0f, clientHeight / 1.10f }, "C", 360, 105, UI::COLOR::GRAY, [this] { Quit(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 8.0f, clientHeight / 1.1f }, "I", "QuitButton.png", 0.6f, 1.0f);

	menuCanvas->AddButton({ clientWidth / 1.25f, clientHeight / 1.15f }, "Form", 480, 150, UI::COLOR::GRAY, [this] { Form(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 1.25f, clientHeight / 1.15f }, "Form", "Form.png", 0.75f, true);

	canvases["MAIN MENU"] = menuCanvas;
	currentCanvas = menuCanvas;

	auto howToPlayCanvas = new Canvas();
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "C", "Controls.png", 2.0f, 1.0f);
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.1f }, "W", "BackButton.png", 0.5f, 1.0f);
	howToPlayCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.1f }, "Q", 340, 90, UI::COLOR::GRAY, [this] { BacktoMenu(); }, Hovering);

	canvases["HOW TO PLAY"] = howToPlayCanvas;

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 370.0f, 180.0f, -90.0 }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, 1, 1);
	
	(void)Run();
}

MainMenu::~MainMenu()
{
	for (auto& [name, canvas] : canvases)
		delete canvas;
}

void MainMenu::Initialize()
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
			//shadowRenderer.Bind(model);
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}

}

void MainMenu::Render()
{
	ShaderData::Inst().BindFrameConstants();

	Graphics::Inst().BeginFrame();

	modelRenderer.Render();

	particleRenderer.Render();

	terrainRenderer.Render(terrain);

	//shadowRenderer.Render();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

APPSTATE MainMenu::Run()
{
	Render();
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	scene.Update();

	currentCanvas->Update();

	if (play)
		return APPSTATE::GAME;

	if (quit)
		return APPSTATE::EXIT;
	
	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;
}
