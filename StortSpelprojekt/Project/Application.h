#pragma once
#include "Window.h"
#include "Event.h"
#include "Time.h"
#include "Game.h"
#include "LoadingScreen.h"
#include "ApplicationState.h"
#include "ShaderData.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "Win.h"
#include "HardwareSupport.h"

class Application
{
private:
	//SINGLETONS
	std::unique_ptr<Renderers> renderers;
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;
	std::unique_ptr<UI> ui;

	APPSTATE currentState;
	ApplicationState* state = nullptr;
public:
	Application(HINSTANCE instance)
	{
		FileSystem::SetProjectDirectory();

		WindowCreator(Window(), 900, 600, L"ARCUS", instance);
		
		graphics = std::make_unique<Graphics>(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND(), false);

		//RunLoadingScreen();

		shaderData = std::make_unique<ShaderData>();

		renderers = std::make_unique<Renderers>();

		resources = std::make_unique<Resources>();

		ui = std::make_unique<UI>();

		//SWAP TO MAINMENU TO NOT SKIP IT
		Window::DeactivateCursor();

		//Audio::Initialize(false);

		state = new MainMenu(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
	}

	~Application()
	{
		ui.reset();
		Window::ShutDown();
		Audio::StopEngine();
	}

	int Run()
	{
		Timer timer;
		float FPS = 60.0f;
		float tickInterval = 1.0f / FPS;

		MSG msg = {};

		while (currentState != APPSTATE::EXIT)
		{
			timer.Start();

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (Window::Exit())
			{
				delete state;
				break;
			}

			currentState = state->Run();

			switch (currentState)
			{

			case APPSTATE::NO_CHANGE:
				break;

			case APPSTATE::MAIN_MENU:
				RND.ShutDown();
				delete state;
				RunLoadingScreen();
				state = new MainMenu(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
				break;

			case APPSTATE::WIN:
				RND.ShutDown();
				delete state;
				RunLoadingScreen();
				state = new Win(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
				break;

			case APPSTATE::GAMEOVER:
				RND.ShutDown();
				delete state;
				RunLoadingScreen();
				state = new GameOver(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
				break;

			case APPSTATE::GAME:
				RND.ShutDown();
				delete state;
				RunLoadingScreen();
				state = new Game(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
				break;

			case APPSTATE::EXIT:
				delete state;
				break;
			}

			float dt = timer.DeltaTime();

			if (dt < tickInterval)
			{
				float timeToSleep = tickInterval - dt;
				Sleep(timeToSleep * 1000.0f);
				dt += timeToSleep;
			}

			Time::Update(dt);
		}

		return 0;
	}
};