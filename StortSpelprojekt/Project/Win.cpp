#include "Win.h"

void Win::MainMenu()
{
	backToMenu = true;
}

void Win::BackToMainMenu()
{
	currentCanvas = canvases["WIN"];
}

void Win::HoveringMainMenu()
{

	canvases["WIN"]->GetImage("MainMenuLeaves")->Show();
}

void Win::Form()
{
	ShellExecute(0, 0, L"https://docs.google.com/forms/d/1wSGU7CwBNTTCu50nsunQX2Q9DC06SEi5SAqbgJstgb0/viewform?edit_requested=true", 0, 0, SW_SHOW);
}
	currentCanvas = new Canvas();

void Win::HoveringForm()
{
	canvases["WIN"]->GetImage("FormLeaves")->Show();
}

void Win::QuitCanvas()
{
	currentCanvas = canvases["QUIT"];
}

void Win::HoveringYes()
{
	canvases["QUIT"]->GetImage("YesLeaves")->Show();

}

void Win::HoveringNo()
{
	canvases["QUIT"]->GetImage("NoLeaves")->Show();
}

Win::Win(UINT clientWidth, UINT clientHeight, HWND window)
	:water(5000), terrain(2)
{
	RND.InitModelRenderer();
	RND.InitParticleRenderer();
	RND.InitShadowRenderer();
	RND.InitTerrainRenderer();
	RND.InitWaterRenderer();
	Audio::AddAudio(L"Audio/Win.wav",0);
	Audio::SetVolume(0.005,0);
	Audio::StartAudio(0);
	
	Initialize();
	float xPos = 75;
	auto winCanvas = std::make_shared<Canvas>();
	winCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f - 250 }, "ThankYou", "ThankYou.png", 1.0f, 1.0f, true, true);

	{
		//MAIN MENU
		winCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight / 2.0f + 300.0f}, "MainMenu", "MainMenu.png", 0.95f, 1.0f, true, true);
		winCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight / 2.0f + 300.0f }, "MainMenuLeaves", "MainMenuLeaves.png", 0.95f, 1.0f, true, true);
		auto image = winCanvas->GetImage("MainMenu");
		winCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "MainMenuButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { QuitCanvas(); }, [this] { HoveringMainMenu(); });
	}
	{
		// FORM
		winCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight / 2.0f + 225.0f }, "Form", "Form.png", 0.95f, 1.0f, true, true);
		winCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight / 2.0f + 225.0f }, "FormLeaves", "FormLeaves.png", 0.95f, 1.0f, true, true);
		auto image = winCanvas->GetImage("Form");
		winCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "FormButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { Form(); }, [this] { HoveringForm(); });
	}
	canvases["WIN"] = winCanvas;

	auto quitGameCanvas = std::make_shared<Canvas>();
	{
		//YES NO
		quitGameCanvas->AddImage({ clientWidth / 2.0f,  clientHeight / 2.0f - 200 }, "AreYouSure", "AreYouSure.png", 1.f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50 }, "Yes", "Yes.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50 }, "YesLeaves", "YesLeaves.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50 }, "No", "No.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50 }, "NoLeaves", "NoLeaves.png", 1.0f, 1.0f, true, true);
		auto yesImage = quitGameCanvas->GetImage("Yes");
		auto noImage = quitGameCanvas->GetImage("No");
		quitGameCanvas->AddButton({ yesImage->GetLeftSidePosition().x + yesImage->GetWidth() / 2, yesImage->GetLeftSidePosition().y + yesImage->GetHeight() / 2 }, "YesButton", yesImage->GetWidth(), yesImage->GetHeight(), UI::COLOR::GRAY, [this] { MainMenu(); }, [this] { HoveringYes(); });
		quitGameCanvas->AddButton({ noImage->GetLeftSidePosition().x + noImage->GetWidth() / 2, noImage->GetLeftSidePosition().y + noImage->GetHeight() / 2 }, "NoButton", noImage->GetWidth(), noImage->GetHeight(), UI::COLOR::GRAY, [this] { BackToMainMenu(); }, [this] { HoveringNo(); });
	}
	canvases["QUIT"] = quitGameCanvas;

	currentCanvas = winCanvas;

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 38.055f, 20.367f, -594.542f }, { 0.f, 1.f, 0.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, { 1.0f, 1.0f, 1.0f, 1 }, 4, 4);
	scene.AddPointLight({ -42.f, 40.0f, -687.4f }, 60, { 0.2f, 0.2f, 0.2f }, { 255.0f / 255.0f, 55.0f / 255.0f, 42.0f / 255.0f, 1.0f });

	//186 95 42 
	auto menuFireSystem = std::make_shared<ParticleSystem>("MainMenuPS.ps");
	scene.AddParticleSystem("MenuFireSystem", menuFireSystem, Vector3{ 38.055f, 20.367f, -594.542f });
	PR->Bind(menuFireSystem);

	(void)Run();
}

Win::~Win()
{
	RND.ShutDown();
}

void Win::Render()
{
	SR->Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	PR->Render();

	MR->Render();

	TR->Render(terrain);

	WR->Render(water);

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
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
			MR->Bind(model);
			SR->Bind(model);
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

APPSTATE Win::Run()
{
	// LEAVES
	canvases["WIN"]->GetImage("MainMenuLeaves")->Hide();
	canvases["WIN"]->GetImage("FormLeaves")->Hide();
	canvases["QUIT"]->GetImage("YesLeaves")->Hide();
	canvases["QUIT"]->GetImage("NoLeaves")->Hide();

	currentCanvas->Update();
	scene.GetCamera()->RotateAroundPoint({ -27.991f, 17.551f, -632.529f }, 150, (Vector3{ 0, -0.6f, -1 } / Vector3(0, -0.6f, -1).Length()));
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	scene.Update();
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), scene.GetPointLights());

	Render();

	if (backToMenu)
		return APPSTATE::MAIN_MENU;

	return APPSTATE::NO_CHANGE;
}
