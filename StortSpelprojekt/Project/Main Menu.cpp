#include "Main Menu.h"

void MainMenu::Options()
{
	currentCanvas = canvases["OPTIONS"];
}

void MainMenu::HowToPlay()
{

	currentCanvas = canvases["HOWTOPLAY"];
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
void Hovering()
{
	Print("HOVERING");
}

 
MainMenu::MainMenu(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED),
	terrainRenderer(DEFERRED, 40),
	deferredRenderer(clientWidth, clientHeight)
{
<<<<<<< HEAD
	ui = std::make_unique<UI>(window);

	auto menuCanvas = new Canvas();
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 5.0f }, "K", "Arcus logo.png", 0.75f, 1.0f);
	menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.10f }, "O", 360, 100, UI::COLOR::GRAY, [this] { Play(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.11f }, "L", "MainMenuPlay.png", 0.6f, 1.0f);
	//menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.7f }, "A", 370, 133, UI::COLOR::GRAY, [this] { HowToPlay(); }, Hovering);
	menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.8f }, "N", 360, 100, UI::COLOR::GRAY, [this] { Options(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.8f }, "J", "MainMenuOptions.png", 0.6f, 1.0f);
	menuCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.49f }, "B", 270, 100, UI::COLOR::GRAY, [this] { HowToPlay(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.49f }, "H", "MainMenuControls.png", 0.6f, 1.0f);
	menuCanvas->AddButton({ clientWidth / 5.0f, clientHeight / 1.10f }, "C", 270, 100, UI::COLOR::GRAY, [this] { Quit(); }, Hovering);
	menuCanvas->AddImage({ clientWidth / 5.0f, clientHeight / 1.1f }, "I", "MainMenuQuit.png", 0.6f, 1.0f);
	canvases["MAIN MENU"] = menuCanvas;
	currentCanvas = menuCanvas;

	auto optionsCanvas = new Canvas();
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "X", "Options.png", 1.0f, 1.0f);
	optionsCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.08f }, "D", 200, 78, UI::COLOR::GRAY, [this] { BacktoMenu(); }, Hovering);

	canvases["OPTIONS"] = optionsCanvas;

	Initialize();

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -20.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(50, 4, 4);
	auto model = std::make_shared<Model>("Oak", "testtest");
	scene.AddModel("testtest", model);
	model.get()->SetPosition(10, 0, 10);
	modelRenderer.Bind(model);
	shadowRenderer.Bind(model);

	auto rainSystem = std::make_shared<ParticleSystem>("rain.ps");
	scene.AddParticleSystem("Rain", rainSystem, Vector3{ 0,30,0 });
	particleRenderer.Bind(rainSystem);

	auto fireSystem = std::make_shared<ParticleSystem>("fire.ps");
	scene.AddParticleSystem("fire", fireSystem, Vector3{ 0,-3,0 });
	particleRenderer.Bind(fireSystem);

	(void)Run();
=======
	currentCanvas = new Canvas();
	currentCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight }, "juan.png", "CompassBase.png");
	
>>>>>>> main
}





MainMenu::~MainMenu()
{
	delete currentCanvas;
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
			shadowRenderer.Bind(model);
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}

}

<<<<<<< HEAD
void MainMenu::Render()
{
	deferredRenderer.SetRenderTargets();
	modelRenderer.Render();
	particleRenderer.Render();
	terrainRenderer.Render(terrain);
	shadowRenderer.Render();
	Graphics::Inst().BeginFrame();
	deferredRenderer.Render();
	currentCanvas->Render();
	Graphics::Inst().EndFrame();
	Resources::Inst().Reset();
}

State MainMenu::Run()
{
	scene.Update();
	currentCanvas->Update();
	Render();
	
	if (play)
		return State::GAME;
=======
APPSTATE MainMenu::Run()
{
	Graphics::Inst().BeginFrame();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	if (Event::KeyIsPressed('G'))
		return APPSTATE::GAME;
>>>>>>> main

	if (quit)
		return State::EXIT;
	
	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;
}
