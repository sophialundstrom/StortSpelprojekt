#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"
#include "States.h"

class Application
{
private:
	//WINDOW
	Window window;

	//SINGLETONS
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;

	//STATES
	State currentState;
	GameState* currentGameState;
public:
	Application(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
		:window(width, height, title, instance)
	{
		FileSystem::SetProjectDirectory();

		//INITIALIZATION
		UINT clientWidth = window.ClientWidth();
		UINT clientHeight = window.ClientHeight();

		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, window.GetHWND());
		shaderData = std::make_unique<ShaderData>("");
		resources = std::make_unique<Resources>();

		ImGUI::Initialize();

		//STATES
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

			if (Event::KeyIsPressed('C'))
				window.ToggleCursor();

			if (!window.CursorIsActive())
				SetCursorPos(window.ClientWidth() / 2, window.ClientHeight() / 2);

			if (Event::KeyIsPressed(VK_RETURN))
				break;
			
			currentState = currentGameState->Run();

			switch (currentState)
			{
			case State::NO_CHANGE:
				break;

			case State::MENU:
				currentGameState->Delete();
				currentGameState = new DebugMainMenu(window.ClientWidth(), window.ClientHeight());
				break;

			case State::GAME:
				currentGameState->Delete();
				currentGameState = new Game(window.ClientWidth(), window.ClientHeight());
				break;

			case State::LEVEL:
				currentGameState->Delete();
				currentGameState = new LevelEditor(window.ClientWidth(), window.ClientHeight());
				break;

			case State::PARTICLE:
				currentGameState->Delete();
				currentGameState = new ParticleEditor(window.ClientWidth(), window.ClientHeight());
				break;

			case State::EXIT:
				Print("CLOSING APPLICATION");
				break;
			}

			Time::Update(timer.DeltaTime());
		}
	}
};