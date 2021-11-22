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
	:modelRenderer(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD),
	water(5000), terrain(2)
{
	Audio::AddAudio(L"Audio/GameOver.wav", 0);
	Audio::SetVolume(0.005, 0);
	Audio::StartAudio(0);

	Initialize();

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

	auto menuFireSystem = std::make_shared<ParticleSystem>("MainMenuPS.ps");
	scene.AddParticleSystem("MenuFireSystem", menuFireSystem, Vector3{ -42, 35, -687 });
	particleRenderer.Bind(menuFireSystem);

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { -41.0f, 37.0f, -687.0f }, { 0.f, 1.f, 0.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(200, { 0.03f, 0.03f, 0.03f ,1 }, 1);
	scene.AddPointLight({ -42.f, 40.0f, -687.4f }, 60, { 0.2f, 0.2f, 0.2f }, { 255.0f / 255.0f, 55.0f / 255.0f, 42.0f / 255.0f, 1.0f });

	(void)Run();

	//BindShaders(nullptr, nullptr, nullptr, nullptr, nullptr);
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
	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	particleRenderer.Render();

	modelRenderer.Render();

	terrainRenderer.Render(terrain);

	//waterRenderer.Render(water);

	//skeletonRenderer.Render();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void GameOver::Initialize()
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("MainMenu", scene.GetDrawables());

	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			shadowRenderer.Bind(model);
			continue;
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
			continue;
		}
	}
}

APPSTATE GameOver::Run()
{
	scene.Update();
	scene.GetCamera()->RotateAroundPoint({ -41.0f, 37.0f, -687.0f }, 40, (Vector3{ 0, -0.6f, -1 } / Vector3(0, -0.6f, -1).Length()));
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), scene.GetPointLights());

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