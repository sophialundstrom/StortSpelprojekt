#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"
#include "States.h"
#include "LoadingScreen.h"
#include "Main Menu.h"

class Application
{
private:
	//WINDOW
	Window* window;

	//SINGLETONS
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;

	//STATES
	State currentState;
	GameState* currentGameState;

	const std::wstring title = L"Stort Spelprojekt";
	bool isFullscreen = false;
	const UINT width = 1280;
	const UINT height = 720;
	const UINT fullScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	const UINT fullscreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HINSTANCE instance;

	void InitFullscreen()
	{ 
		if (window)
			delete window;

		window = new Window(fullScreenWidth, fullscreenHeight, title.c_str(), instance);

		isFullscreen = true;

		UINT clientWidth = window->ClientWidth();
		UINT clientHeight = window->ClientHeight();

		graphics.reset();
		shaderData.reset();
		resources.reset();

		ImGUI::ShutDown();

		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, window->GetHWND(), false);

		RunLoadingScreen();

		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();

		ImGUI::Initialize();
	}

	void InitWindowed()
	{
		if (window)
			delete window;

		window = new Window(width, height, title.c_str(), instance);

		isFullscreen = false;

		UINT clientWidth = window->ClientWidth();
		UINT clientHeight = window->ClientHeight();

		graphics.reset();
		shaderData.reset();
		resources.reset();

		ImGUI::ShutDown();

		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, window->GetHWND());
		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();

		ImGUI::Initialize();
	}
public:
	Application(HINSTANCE instance)
	{
		FileSystem::SetProjectDirectory();
		InitWindowed();

		UINT clientWidth = window->ClientWidth();
		UINT clientHeight = window->ClientHeight();

		currentState = State::MENU;
		currentGameState = new DebugMainMenu(clientWidth, clientHeight);
	}

	~Application()
	{
		ImGUI::ShutDown();
	}

	void Run()
	{
		Timer timer;

		MSG msg = {};

		while (currentState != State::EXIT)
		{
			timer.Start();

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (window->Exit())
			{
				currentGameState->Delete();
				break;
			}
				
			if (Event::KeyIsPressed(VK_TAB))
				window->ToggleCursor();
			
			currentState = currentGameState->Run();

			switch (currentState)
			{
			case State::NO_CHANGE:
				break;

			case State::MENU:
				currentGameState->Delete();

				if (isFullscreen)
					InitWindowed();
				
				currentGameState = new DebugMainMenu(window->ClientWidth(), window->ClientHeight());
				break;

			case State::GAME:
				delete currentGameState;
				InitFullscreen();
				RunLoadingScreen();
				currentGameState = new Game(window->ClientWidth(), window->ClientHeight(), window->GetHWND());

				break;

			case State::MAIN_MENU:
				delete currentGameState;
				RunLoadingScreen();
				currentGameState = new MainMenu(window->ClientWidth(), window->ClientHeight(), window->GetHWND());

				break;

			case State::LEVEL:
				RunLoadingScreen();
				currentGameState->Delete();
				currentGameState = new LevelEditor(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case State::PARTICLE:
				currentGameState->Delete();
				currentGameState = new ParticleEditor(window->ClientWidth(), window->ClientHeight());
				break;

			case State::EXIT:
				currentGameState->Delete();
				break;
			}
			Time::Update(timer.DeltaTime());
		}
	}
};