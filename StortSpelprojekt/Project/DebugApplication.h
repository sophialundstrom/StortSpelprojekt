#pragma once
#include "Window.h"
#include "Event.h"
#include "Time.h"
#include "States.h"
#include "LoadingScreen.h"
#include "MainMenu.h"
#include "Renderers.h"

class DebugApplication
{
private:
	//SINGLETONS
	std::unique_ptr<Renderers> renderers;
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

		Window::Resize();
		Window::ActivateCursor();

		isFullscreen = true;

		UINT clientWidth = Window::ClientWidth();
		UINT clientHeight = Window::ClientHeight();

		graphics.reset();
		shaderData.reset();
		resources.reset();

		ImGUI::ShutDown();

		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, Window::GetHWND(), false);

		RunLoadingScreen();

		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();
		ui = std::make_unique<UI>();

		ImGUI::Initialize();
	}

	void InitWindowed()
	{
		ui.reset();;

		Window::Resize(width, height);

		isFullscreen = false;

		UINT clientWidth = Window::ClientWidth();
		UINT clientHeight = Window::ClientHeight();

		graphics.reset();
		shaderData.reset();
		resources.reset();

		ImGUI::ShutDown();

		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, Window::GetHWND());
		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();
		ui = std::make_unique<UI>();

		ImGUI::Initialize();
	}
public:
	DebugApplication(HINSTANCE instance)
	{
		FileSystem::SetProjectDirectory();

		WindowCreator(Window(), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), L"ARCUS EDITOR", instance);

		InitWindowed();

		UINT clientWidth = Window::ClientWidth();
		UINT clientHeight = Window::ClientHeight();

		Window::ActivateCursor();
		currentState = APPSTATE::MAIN_MENU;
		currentGameState = new DebugMainMenu(clientWidth, clientHeight);
		renderers = std::make_unique<Renderers>();
	}

	~DebugApplication()
	{
		ui.reset();
		Window::ShutDown();
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

			if (Window::Exit())
			{
				currentGameState->Delete();
				break;
			}
				
			if (Event::KeyIsPressed(VK_TAB))
				Window::ActivateCursor();

			currentState = currentGameState->Run();

			switch (currentState)
			{
			case APPSTATE::NO_CHANGE:
				break;

			case APPSTATE::MAIN_MENU:
				currentGameState->Delete();
				Window::ActivateCursor();
				InitWindowed();
				currentGameState = new DebugMainMenu(Window::ClientWidth(), Window::ClientHeight());
				break;

			case APPSTATE::GAME:
				delete currentGameState;
				Window::DeactivateCursor();
				return 1;
				break;

			case APPSTATE::LEVEL:
				RunLoadingScreen();
				currentGameState->Delete();
				Window::ActivateCursor();
				InitFullscreen();
				currentGameState = new LevelEditor(Window::ClientWidth(), Window::ClientHeight(), Window::GetHWND());
				break;

			case APPSTATE::PARTICLE:
				currentGameState->Delete();
				Window::ActivateCursor();
				InitFullscreen();
				currentGameState = new ParticleEditor(Window::ClientWidth(), Window::ClientHeight());
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