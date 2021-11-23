#include "GameOver.h"

void hovering()
{
	Print("Hovering");
}

void hovering2()
{
	Print("Quit");
}
void
GameOver::Form()
{
	ShellExecute(0, 0, L"https://docs.google.com/forms/d/1wSGU7CwBNTTCu50nsunQX2Q9DC06SEi5SAqbgJstgb0/viewform?edit_requested=true", 0, 0, SW_SHOW);
}

void GameOver::HoveringForm()
{
	canvases["GAMEOVER"]->GetImage("FormLeaves")->Show();
}

void GameOver::Exit()
{
	quit = true;
}

void GameOver::Back()
{
	currentCanvas = canvases["GAMEOVER"];
}

void GameOver::HoveringYes()
{
	if (currentCanvas == canvases["QUIT"])
	{
		canvases["QUIT"]->GetImage("YesLeaves")->Show();
	}
	else if (currentCanvas == canvases["MAINMENU"])
	{
		canvases["MAINMENU"]->GetImage("YesLeaves")->Show();
	}
}

void GameOver::HoveringNo()
{
	if (currentCanvas == canvases["QUIT"])
	{
		canvases["QUIT"]->GetImage("NoLeaves")->Show();
	}
	else if (currentCanvas == canvases["MAINMENU"])
	{
		canvases["MAINMENU"]->GetImage("NoLeaves")->Show();
	}
}

void GameOver::MainMenu()
{
	currentCanvas = canvases["MAINMENU"];
}

void GameOver::BackToMainMenu()
{
	backToMenu = true;
}

void GameOver::HoveringMainMenu()
{
	canvases["GAMEOVER"]->GetImage("MainMenuLeaves")->Show();
}

void GameOver::QuitGame()
{
	currentCanvas = canvases["QUIT"];
}

void GameOver::HoveringQuit()
{
	canvases["GAMEOVER"]->GetImage("QuitLeaves")->Show();
}

void GameOver::Continue()
{
	play = true;
}

void GameOver::HoveringContinue()
{
	canvases["GAMEOVER"]->GetImage("ContinueLeaves")->Show();
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
	float xPos = 75;
	auto gameOverCanvas = std::make_shared<Canvas>();
	gameOverCanvas->AddImage({ clientWidth / 2.0f, 150.0f }, "Form", "Form.png", 1.f, true, true);

	{
		//FORM
		gameOverCanvas->AddImage({ clientWidth - 250.0f, (float)clientHeight / 2.0f + 450}, "Form", "Form.png", 1.f, 1.0f, true, false);
		gameOverCanvas->AddImage({ clientWidth - 250.0f, (float)clientHeight / 2.0f + 450 }, "FormLeaves", "FormLeaves.png", 1.f, 1.0f, true, false);
		auto image = gameOverCanvas->GetImage("Form");
		gameOverCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "FormButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { Form(); }, [this] { HoveringForm(); });

	}
	{
		// QUIT
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f + 150 }, "Quit", "Quit.png", 1.f, 1.0f, true, false);
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f + 150 }, "QuitLeaves", "QuitLeaves.png", 1.f, 1.0f, true, false);
		auto image = gameOverCanvas->GetImage("Quit");
 		gameOverCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "QuitButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { QuitGame(); }, [this] { HoveringQuit(); });
	}
	{
		// QUIT
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f + 75.0f }, "MainMenu", "MainMenu.png", 0.95f, 1.0f, true, false);
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f + 75.0f }, "MainMenuLeaves", "MainMenuLeaves.png", 0.95f, 1.0f, true, false);
		auto image = gameOverCanvas->GetImage("MainMenu");
		gameOverCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "MainMenuButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { MainMenu(); }, [this] { HoveringMainMenu(); });
	}
	{
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f }, "Continue", "Continue.png", 1.f, 1.0f, true, false);
		gameOverCanvas->AddImage({ xPos, (float)clientHeight / 2.0f }, "ContinueLeaves", "ContinueLeaves.png", 1.f, 1.0f, true, false);
		auto image = gameOverCanvas->GetImage("Continue");
		gameOverCanvas->AddButton({ image->GetLeftSidePosition().x + image->GetWidth() / 2, image->GetLeftSidePosition().y + image->GetHeight() / 2 }, "ContinueButton", image->GetWidth(), image->GetHeight(), UI::COLOR::GRAY, [this] { Continue(); }, [this] { HoveringContinue(); });
	}
	{
		gameOverCanvas->AddImage({ 25, clientHeight / 2.0f }, "MenuStick", "MenuStick.png", 0.8f, 1.0f, true, true);
		gameOverCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight / 7.0f }, "GameOver", "GameOver.png", 0.8f, 1.0f, true, true);
	}
	canvases["GAMEOVER"] = gameOverCanvas;
	currentCanvas = gameOverCanvas;

	auto quitGameCanvas = std::make_shared<Canvas>();
	{
		quitGameCanvas->AddImage({ clientWidth / 2.0f,  clientHeight / 2.0f - 200 }, "AreYouSure", "AreYouSure.png", 1.f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50}, "Yes", "Yes.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50}, "YesLeaves", "YesLeaves.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50}, "No", "No.png", 1.0f, 1.0f, true, true);
		quitGameCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50}, "NoLeaves", "NoLeaves.png", 1.0f, 1.0f, true, true);
		auto yesImage = quitGameCanvas->GetImage("Yes");
		auto noImage = quitGameCanvas->GetImage("No");
		quitGameCanvas->AddButton({ yesImage->GetLeftSidePosition().x + yesImage->GetWidth() / 2, yesImage->GetLeftSidePosition().y + yesImage->GetHeight() / 2 }, "YesButton", yesImage->GetWidth(), yesImage->GetHeight(), UI::COLOR::GRAY, [this] { Exit(); }, [this] { HoveringYes(); });
		quitGameCanvas->AddButton({ noImage->GetLeftSidePosition().x + noImage->GetWidth() / 2, noImage->GetLeftSidePosition().y + noImage->GetHeight() / 2 }, "NoButton", noImage->GetWidth(), noImage->GetHeight(), UI::COLOR::GRAY, [this] { Back(); }, [this] { HoveringNo(); });

	}
	canvases["QUIT"] = quitGameCanvas;

	auto mainMenuCanvas = std::make_shared<Canvas>();
	{
		mainMenuCanvas->AddImage({ clientWidth / 2.0f,  clientHeight / 2.0f - 200 }, "AreYouSure", "AreYouSure.png", 1.f, 1.0f, true, true);
		mainMenuCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50 }, "Yes", "Yes.png", 1.0f, 1.0f, true, true);
		mainMenuCanvas->AddImage({ clientWidth / 2.0f + 180, clientHeight / 2.0f + 50 }, "YesLeaves", "YesLeaves.png", 1.0f, 1.0f, true, true);
		mainMenuCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50 }, "No", "No.png", 1.0f, 1.0f, true, true);
		mainMenuCanvas->AddImage({ clientWidth / 2.0f - 180, clientHeight / 2.0f + 50 }, "NoLeaves", "NoLeaves.png", 1.0f, 1.0f, true, true);
		auto yesImage = mainMenuCanvas->GetImage("Yes");
		auto noImage = mainMenuCanvas->GetImage("No");
		mainMenuCanvas->AddButton({ yesImage->GetLeftSidePosition().x + yesImage->GetWidth() / 2, yesImage->GetLeftSidePosition().y + yesImage->GetHeight() / 2 }, "YesButton", yesImage->GetWidth(), yesImage->GetHeight(), UI::COLOR::GRAY, [this] { BackToMainMenu(); }, [this] { HoveringYes(); });
		mainMenuCanvas->AddButton({ noImage->GetLeftSidePosition().x + noImage->GetWidth() / 2, noImage->GetLeftSidePosition().y + noImage->GetHeight() / 2 }, "NoButton", noImage->GetWidth(), noImage->GetHeight(), UI::COLOR::GRAY, [this] { Back(); }, [this] { HoveringNo(); });
	}
	canvases["MAINMENU"] = mainMenuCanvas;

	auto menuFireSystem = std::make_shared<ParticleSystem>("MainMenuPS.ps");
	scene.AddParticleSystem("MenuFireSystem", menuFireSystem, Vector3{ -42, 35, -687 });
	particleRenderer.Bind(menuFireSystem);

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { -41.0f, 37.0f, -687.0f }, { 0.f, 1.f, 0.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(200, { 0.03f, 0.03f, 0.03f ,1 }, 1);
	scene.AddPointLight({ -42.f, 40.0f, -687.4f }, 60, { 0.2f, 0.2f, 0.2f }, { 255.0f / 255.0f, 55.0f / 255.0f, 42.0f / 255.0f, 1.0f });

	(void)Run();
}

GameOver::~GameOver()
{
	
}

void GameOver::Render()
{
	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	particleRenderer.Render();

	modelRenderer.Render();

	//terrainRenderer.Render(terrain);

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
	canvases["GAMEOVER"]->GetImage("FormLeaves")->Hide();
	canvases["GAMEOVER"]->GetImage("QuitLeaves")->Hide();
	canvases["GAMEOVER"]->GetImage("MainMenuLeaves")->Hide();
	canvases["GAMEOVER"]->GetImage("ContinueLeaves")->Hide();
	canvases["QUIT"]->GetImage("YesLeaves")->Hide();
	canvases["QUIT"]->GetImage("NoLeaves")->Hide();
	canvases["MAINMENU"]->GetImage("NoLeaves")->Hide();
	canvases["MAINMENU"]->GetImage("YesLeaves")->Hide();



	scene.Update();
	scene.GetCamera()->RotateAroundPoint({ -41.0f, 37.0f, -687.0f }, 40, (Vector3{ 0, -0.6f, -1 } / Vector3(0, -0.6f, -1).Length()));
	scene.UpdateDirectionalLight(scene.GetCamera()->GetPosition());
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), scene.GetPointLights());

	currentCanvas->Update();
	Render();

	if (quit)
		return APPSTATE::EXIT;

	if (play)
		return APPSTATE::GAME;

	if (backToMenu)
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;;
}