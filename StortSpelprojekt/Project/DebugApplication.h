#pragma once
#include "Window.h"
#include "Event.h"
#include "Time.h"
#include "States.h"
#include "LoadingScreen.h"
#include "Main Menu.h"

class DebugApplication
{
private:
	//WINDOW
	Window* window;

	//SINGLETONS
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;
	std::unique_ptr<UI> ui;

	//STATES
	APPSTATE currentState;
	ApplicationState* currentGameState;

	const std::wstring title = L"Stort Spelprojekt";
	bool isFullscreen = false;
	const UINT width = 1280;
	const UINT height = 720;
	const UINT fullScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	const UINT fullscreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HINSTANCE instance;

	void InitFullscreen()
	{ 
		ui.reset();
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
		ui = std::make_unique<UI>(window->GetHWND());

		ImGUI::Initialize();
	}

	void InitWindowed()
	{
		ui.reset();
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
		ui = std::make_unique<UI>(window->GetHWND());

		ImGUI::Initialize();
	}
public:
	DebugApplication(HINSTANCE instance)
	{
		FileSystem::SetProjectDirectory();
		InitWindowed();

		UINT clientWidth = window->ClientWidth();
		UINT clientHeight = window->ClientHeight();

		currentState = APPSTATE::MAIN_MENU;
		currentGameState = new DebugMainMenu(clientWidth, clientHeight);
	}

	~DebugApplication()
	{
		ui.reset();
		delete window;
		ImGUI::ShutDown();
	}

	int Run()
	{
		Timer timer;

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
				currentGameState->Delete();
				break;
			}
				
			if (Event::KeyIsPressed(VK_TAB))
				window->ToggleCursor();

			currentState = currentGameState->Run();

			switch (currentState)
			{
			case APPSTATE::NO_CHANGE:
				break;

			case APPSTATE::MAIN_MENU:
				currentGameState->Delete();
				InitWindowed();
				currentGameState = new DebugMainMenu(window->ClientWidth(), window->ClientHeight());
				break;

			case APPSTATE::GAME:
				delete currentGameState;
				return 1;
				break;

			case APPSTATE::LEVEL:
				RunLoadingScreen();
				currentGameState->Delete();
				InitFullscreen();
				currentGameState = new LevelEditor(window->ClientWidth(), window->ClientHeight(), window->GetHWND());
				break;

			case APPSTATE::PARTICLE:
				currentGameState->Delete();
				InitFullscreen();
				currentGameState = new ParticleEditor(window->ClientWidth(), window->ClientHeight());
				break;

			case APPSTATE::EXIT:
				currentGameState->Delete();
				break;
			}

			Time::Update(timer.DeltaTime());
		}

		return 0;
	}
};