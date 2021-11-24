#include "Win.h"

void Win::MainMenu()
{
	backToMenu = true;
}

void Win::BackToMainMenu()
{
}

void Win::HoveringMainMenu()
{
	canvases[""]->GetImage("Leaves")->Show();

}

void Win::QuitGame()
{
	quit = true;
}

void Win::HoveringQuit()
{
	canvases[""]->GetImage("Leaves")->Show();

}

void Win::Form()
{
}

void Win::HoveringForm()
{
	canvases[""]->GetImage("FormLeaves")->Show();
}

void Win::Exit()
{
}

void Win::Back()
{
}

void Win::HoveringYes()
{
	canvases[""]->GetImage("Leaves")->Show();

}

void Win::HoveringNo()
{
	canvases[""]->GetImage("Leaves")->Show();

}

Win::Win(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD),
	water(5000), terrain(2)
{

	Audio::AddAudio(L"Audio/Win.wav",0);
	Audio::SetVolume(0.005,0);
	Audio::StartAudio(0);
	
	Initialize();
	float xPos = 75;
	auto winCanvas = new Canvas();
	winCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f - 250 }, "ThankYou", "ThankYou.png", 1.0f, 1.0f, true, true);

	canvases["WIN"] = winCanvas;
	currentCanvas = winCanvas;

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { -41.0f, 37.0f, -687.0f }, { 0.f, 1.f, 0.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(200, { 0.03f, 0.03f, 0.03f ,1 }, 1);
	scene.AddPointLight({ -42.f, 40.0f, -687.4f }, 60, { 0.2f, 0.2f, 0.2f }, { 255.0f / 255.0f, 55.0f / 255.0f, 42.0f / 255.0f, 1.0f });

	//186 95 42 
	auto menuFireSystem = std::make_shared<ParticleSystem>("MainMenuPS.ps");
	scene.AddParticleSystem("MenuFireSystem", menuFireSystem, Vector3{ -42, 35, -687 });
	particleRenderer.Bind(menuFireSystem);

	(void)Run();
}

Win::~Win()
{
	for (auto& [name, canvas] : canvases)
		delete canvas;
}

void Win::Render()
{
	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	modelRenderer.Render();

	particleRenderer.Render();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void Win::Initialize()
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

void Win::Continue()
{
}

void Win::HoveringContinue()
{
}

APPSTATE Win::Run()
{
	// LEAVES

	currentCanvas->Update();
	scene.GetCamera()->RotateAroundPoint({ -41.0f, 37.0f, -687.0f }, 40, (Vector3{ 0, -0.6f, -1 } / Vector3(0, -0.6f, -1).Length()));
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	scene.Update();
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), scene.GetPointLights());

	Render();


	if (Event::KeyIsPressed('M'))
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	if (quit)
		return APPSTATE::EXIT;

	if (backToMenu)
		return APPSTATE::MAIN_MENU;


	return APPSTATE::NO_CHANGE;
}
