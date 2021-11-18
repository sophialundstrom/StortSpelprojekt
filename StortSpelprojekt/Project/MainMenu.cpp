#include "MainMenu.h"

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
	ShellExecute(0, 0, L"https://docs.google.com/forms/d/1wSGU7CwBNTTCu50nsunQX2Q9DC06SEi5SAqbgJstgb0/viewform?edit_requested=true", 0, 0, SW_SHOW);
}

void MainMenu::HoveringContinue()
{
	canvases["MAIN MENU"]->GetImage("ContinueLeaves")->Show();
}

void MainMenu::HoveringQuit()
{
	canvases["MAIN MENU"]->GetImage("QuitLeaves")->Show();

}

void MainMenu::HoveringHowToPlay()
{
	canvases["MAIN MENU"]->GetImage("HowToPlayLeaves")->Show();

}

void MainMenu::HoveringForm()
{
	canvases["MAIN MENU"]->GetImage("FormLeaves")->Show();
}

void MainMenu::HoveringOptions()
{
	canvases["MAIN MENU"]->GetImage("OptionsLeaves")->Show();

}

void MainMenu::HoveringBackOptions()
{
	canvases["OPTIONS"]->GetImage("BackLeavesOptions")->Show();
}

void MainMenu::HoveringBackControls()
{
	canvases["HOW TO PLAY"]->GetImage("BackLeavesControls")->Show();
}

void MainMenu::HoveringNewGame()
{
	canvases["MAIN MENU"]->GetImage("NewGameLeaves")->Show();
}



void Hovering()
{
	
	//Print("HOVERING");
}

MainMenu::MainMenu(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD, 40)
{
	Initialize();
	
	float Volume = 0.4;
	Audio::AddAudio(L"Audio/Menu.wav", 0);
	Audio::SetVolume(Volume, 0);
	Audio::StartAudio(0);

	auto menuCanvas = new Canvas();
	
	// LOGO
	menuCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 5.0f }, "ArcusLogo", "Logotype.png", 1.0f, 1.0f);

	// NEW GAME
	menuCanvas->AddButton({ clientWidth / 7.5f, clientHeight / 2.0f }, "NewGameButton", 365, 50, UI::COLOR::GRAY, [this] { Play(); }, [this] {HoveringNewGame(); });
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f }, "NewGame", "NewGame.png", 1.f, 1.0f);
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f }, "NewGameLeaves", "NewGameLeaves.png", 1.f, 1.0f);

	// CONTINUE
	menuCanvas->AddButton({ clientWidth / 7.5f, clientHeight / 2.0f - 75 }, "ContinueButton", 375, 50, UI::COLOR::GRAY, [this] { Play(); }, [this] {HoveringContinue(); });
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f - 75 }, "Continue", "Continue.png", 1.f, 1.0f);
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f - 75 }, "ContinueLeaves", "ContinueLeaves.png", 1.f, 1.0f);
	
	// HOW TO PLAY
	menuCanvas->AddButton({ clientWidth / 7.5f, clientHeight / 2.0f + 75 }, "HowToPlayButton", 450, 50, UI::COLOR::GRAY, [this] { HowToPlay(); }, [this] { HoveringHowToPlay(); });
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 75}, "HowToPlay", "HowToPlay.png", 1.0f, 1.0f);
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 75 }, "HowToPlayLeaves", "HowToPlayLeaves.png", 1.0f, 1.0f);

	// OPTIONS
	menuCanvas->AddButton({ clientWidth / 7.5f, clientHeight / 2.0f + 150 }, "OptionsButton", 310, 50, UI::COLOR::GRAY, [this] { Options(); }, [this] { HoveringOptions(); });
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 150 }, "Options", "Options.png", 1.0f, 1.0f);
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 150 }, "OptionsLeaves", "OptionsLeaves.png", 1.0f, 1.0f);

	// QUIT
	menuCanvas->AddButton({ clientWidth / 7.5f, clientHeight / 2.0f + 225}, "QuitButton", 180, 50, UI::COLOR::GRAY, [this] { Quit(); }, [this] { HoveringQuit(); });
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 225}, "Quit", "Quit.png", 1.f, 1.0f);
	menuCanvas->AddImage({ clientWidth / 7.5f, clientHeight / 2.0f + 225}, "QuitLeaves", "QuitLeaves.png", 1.f, 1.0f);

	// FORM
	menuCanvas->AddButton({ clientWidth / 1.1f, clientHeight / 2.0f + 450 }, "FormButton", 200, 50, UI::COLOR::GRAY, [this] { Form(); }, [this] {HoveringForm(); });
	menuCanvas->AddImage({ clientWidth / 1.1f, clientHeight / 2.0f + 450 }, "Form", "Form.png", 1.0f, true);
	menuCanvas->AddImage({ clientWidth / 1.1f, clientHeight / 2.0f + 450 }, "FormLeaves", "FormLeaves.png", 1.0f, true);

	canvases["MAIN MENU"] = menuCanvas; 
	currentCanvas = menuCanvas;

	// HOW TO PLAY CANVAS
	auto howToPlayCanvas = new Canvas();
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "Controls", "Controls.png", 2.0f, 1.0f);

	// BACK
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f + 450 }, "BackControls", "Back.png", 1.0f, 1.0f);
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f + 450 }, "BackLeavesControls", "BackLeaves.png", 1.f, 1.0f);
	howToPlayCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f + 450 }, "BackButtonControls", 180, 50, UI::COLOR::GRAY, [this] { BacktoMenu(); }, [this] { HoveringBackControls(); });

	canvases["HOW TO PLAY"] = howToPlayCanvas;

	// OPTIONS
	auto optionsCanvas = new Canvas();

	// OPTIONS TITLE
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f - 350 }, "OptionsInOptions", "Options.png", 1.5f, 1.0f);
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f - 350 }, "OptionsLeavesInOptions", "OptionsLeaves.png", 1.5f, 1.0f);
	// BACK
	optionsCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.1f }, "BackButtonOptions", 180, 50, UI::COLOR::GRAY, [this] { BacktoMenu(); }, [this] { HoveringBackOptions(); });
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.1f }, "BackLeavesOptions", "BackLeaves.png", 1.0f, 1.0f);
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.1f }, "BackControls", "Back.png", 1.0f, 1.0f);

	canvases["OPTIONS"] = optionsCanvas;

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { -41.0f, 37.0f, -687.0f }, { 0.f, 1.f, 0.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, { 0.2f, 0.2f, 0.2f ,1 }, 1);
	scene.AddPointLight({ -41.9f, 33.0f, -687.4f }, 30, { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f });


	auto menuFireSystem = std::make_shared<ParticleSystem>("MainMenuPS.ps");
	scene.AddParticleSystem("MenuFireSystem", menuFireSystem, Vector3{ -42, 34, -687 });
	particleRenderer.Bind(menuFireSystem);
		
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
	gameLoader.Load("Main Menu", scene.GetDrawables());

	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			//shadowRenderer.Bind(model);
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

void MainMenu::Render()
{
	ShaderData::Inst().BindFrameConstants();

	Graphics::Inst().BeginFrame();

	modelRenderer.Render();

	particleRenderer.Render();

	//terrainRenderer.Render(terrain);

	//shadowRenderer.Render();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

APPSTATE MainMenu::Run()
{
	// LEAVES
	canvases["MAIN MENU"]->GetImage("NewGameLeaves")->Hide();
	canvases["MAIN MENU"]->GetImage("ContinueLeaves")->Hide();
	canvases["MAIN MENU"]->GetImage("QuitLeaves")->Hide();
	canvases["MAIN MENU"]->GetImage("HowToPlayLeaves")->Hide();
	canvases["MAIN MENU"]->GetImage("FormLeaves")->Hide();
	canvases["MAIN MENU"]->GetImage("OptionsLeaves")->Hide();
	canvases["HOW TO PLAY"]->GetImage("BackLeavesControls")->Hide();
	canvases["OPTIONS"]->GetImage("BackLeavesOptions")->Hide();

	currentCanvas->Update();
	scene.GetCamera()->RotateAroundPoint({ -41.0f, 37.0f, -687.0f }, 30, (Vector3{ 0, -0.6f, -1 } / Vector3(0, -0.6f, -1).Length()));
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	scene.Update();
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), (PointLight::Data*)scene.GetPointLights());

	Render();

	if (play)
		return APPSTATE::GAME;

	if (quit)
		return APPSTATE::EXIT;
	
	return APPSTATE::NO_CHANGE;
}
