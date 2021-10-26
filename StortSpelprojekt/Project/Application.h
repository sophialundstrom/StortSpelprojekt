#pragma once
#include "Window.h"
#include "Event.h"
#include "Time.h"
#include "LoadingScreen.h"
#include "ApplicationState.h"
#include "ShaderData.h"

class Application
{
private:
	Window window;

	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;

	APPSTATE currentState = APPSTATE::MAIN_MENU;
	ApplicationState* state = nullptr;
public:
	Application(HINSTANCE instance)
	{
		FileSystem::SetProjectDirectory();

		window = Window(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), L"Arcus", instance);

		graphics = std::make_unique<Graphics>(window.ClientWidth(), window.ClientHeight(), window.GetHWND(), false);

		RunLoadingScreen();

		shaderData = std::make_unique<ShaderData>();
		resources = std::make_unique<Resources>();
	}

	void Run()
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

			if (window.Exit())
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
				RunLoadingScreen();
				delete state;
				break;

			case APPSTATE::IN_GAME:
				RunLoadingScreen();
				delete state;
				break;

			case APPSTATE::EXIT:
				delete state;
				break;
			}

			Time::Update(timer.DeltaTime());
		}

		MSG msg = {};
	}
};