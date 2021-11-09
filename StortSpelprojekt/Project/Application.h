#pragma once
#include "Window.h"
#include "Event.h"
#include "Time.h"
#include "Game.h"
#include "LoadingScreen.h"
#include "ApplicationState.h"
#include "ShaderData.h"
#include "Main Menu.h"
#include "GameOver.h"
#include "Win.h"

class Application
{
private:
	Window* window;

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

		window = new Window(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), L"ARCUS", instance);
		
		graphics = std::make_unique<Graphics>(window->ClientWidth(), window->ClientHeight(), window->GetHWND(), false);

		RunLoadingScreen();

		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();
		ui = std::make_unique<UI>(window->GetHWND());

		//SWAP TO MAINMENU TO NOT SKIP IT
		window->DeactivateCursor();
		state = new MainMenu(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
	}

	~Application()
	{
		ui.reset();
		delete window;
	}

	int Run()
	{
		Timer timer;
		float FPS = 300.0f;
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

			if (window->Exit())
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
				delete state;
				RunLoadingScreen();
				state = new MainMenu(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case APPSTATE::WIN:
				delete state;
				RunLoadingScreen();
				state = new Win(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case APPSTATE::GAMEOVER:
				delete state;
				RunLoadingScreen();
				state = new GameOver(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case APPSTATE::GAME:
				delete state;
				RunLoadingScreen();
				state = new Game(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case APPSTATE::EXIT:
				delete state;
				break;
			}

			float dt = timer.DeltaTime();

			if (dt < tickInterval)
			{
				float timeToSleep = tickInterval - dt;
				Sleep(timeToSleep * 1000);
				dt += timeToSleep;
			}

			Time::Update(dt);
		}

		return 0;
	}
};