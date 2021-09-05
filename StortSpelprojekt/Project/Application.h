#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"

#include "DebugMainMenu.h"
#include "DemoEditor.h"
#include "LevelEditor.h"
#include "ParticleEditor.h"

class Application
{
private:
	//WINDOW
	Window window;
	UINT clientWidth, clientHeight;

	//MAIN MENU
	DebugMainMenu DBMainMenu;

	//EDITORS
	std::unique_ptr<LevelEditor> levelEditor;
	std::unique_ptr<ParticleEditor> particleEditor;

	//SINGLETONS
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<RenderGraph> renderGraph;
	std::unique_ptr<Resources> resources;
	std::unique_ptr<ShaderData> shaderData;

	//STATE
	AppState state = AppState::DB_MAIN;
public:
	Application(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
		:window(width, height, title, instance)
	{
		FileSystem::SetProjectDirectory();

		//GET CLIENT RECT (BECAUSE OF TITLE BAR OFFSET)
		RECT clientRect;
		GetClientRect(window.HWnd(), &clientRect);

		clientWidth = clientRect.right;
		clientHeight = clientRect.bottom;

		//INITIALIZATION
		graphics = std::make_unique<Graphics>(clientWidth, clientHeight, window.HWnd());

		resources = std::make_unique<Resources>();

		renderGraph = std::make_unique<RenderGraph>(shaderData, clientWidth, clientHeight);
	
		levelEditor = std::make_unique<LevelEditor>();

		particleEditor = std::make_unique<ParticleEditor>();

		ImGUI::Initialize();
	}

	~Application()
	{
		ImGUI::ShutDown();
	}

	void Run()
	{
		Timer timer;

		MSG msg = {};
		while (!window.Exit())
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
				SetCursorPos(window.Width() / 2, window.Height() / 2);

			if (Event::KeyIsPressed(VK_RETURN))
				break;
			
			switch (state)
			{
			case AppState::DB_MAIN:
				DBMainMenu.Update();

				if (DBMainMenu.GetState() != state)
				{
					state = DBMainMenu.GetState();
					DBMainMenu.Reset();

					if (state == AppState::DB_LEVEL)
						levelEditor->Initialize(clientWidth, clientHeight);

					if (state == AppState::DB_PARTICLE)
						particleEditor->Initialize(clientWidth, clientHeight);

					break;
				}
					
				DBMainMenu.Render();
				break;

			case AppState::DB_PLAY:
				RenderGraph::Inst().Render();
				Event::ClearRawDelta();
				break;

			case AppState::DB_LEVEL:
				levelEditor->Update();

				if (levelEditor->IsDone())
				{
					state = AppState::DB_MAIN;
					levelEditor->Reset();
					break;
				}
					
				Event::ClearRawDelta();
				levelEditor->Render();
				break;
			
			case AppState::DB_PARTICLE:
				particleEditor->Update();

				if (particleEditor->IsDone())
				{
					state = AppState::DB_MAIN;
					particleEditor->Reset();
					break;
				}
					
				particleEditor->Render();
				break;

			case AppState::DB_EXIT:
				return;
			}

			Time::Update(timer.DeltaTime());
		}
	}
};