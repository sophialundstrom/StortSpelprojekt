#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"

#include "DebugMainMenu.h"
#include "DemoEditor.h"
#include "LevelEditor.h"

class Application
{
private:
	Window window;

	DebugMainMenu DBMainMenu;
	DemoEditor demoEditor;

	std::unique_ptr<LevelEditor> levelEditor;

	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<RenderGraph> renderGraph;
	std::unique_ptr<TempResources> resources;
	std::unique_ptr<ShaderData> shaderData;

	AppState state = DB_MAIN;
public:
	Application(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
		:window(width, height, title, instance)
	{
		//GET CLIENT RECT (TITLE BAR OFFSET)
		RECT clientRect;
		GetClientRect(window.HWnd(), &clientRect);

		UINT windowWidth = clientRect.right;
		UINT windowHeight = clientRect.bottom;

		//INITIALIZATION
		graphics = std::make_unique<Graphics>(windowWidth, windowHeight, window.HWnd());

		resources = std::make_unique<TempResources>();

		renderGraph = std::make_unique<RenderGraph>(shaderData, windowWidth, windowHeight);
	
		levelEditor = std::make_unique<LevelEditor>(windowWidth, windowHeight);

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
			case DB_MAIN:
				DBMainMenu.Update();

				if (DBMainMenu.GetState() != state)
					state = DBMainMenu.GetState();

				DBMainMenu.Render();
				break;

			case DB_PLAY:
				RenderGraph::Inst().Render();
				break;

			case DB_LEVEL:
				levelEditor->Update();

				if (levelEditor->IsDone())
					state = DB_MAIN;

				levelEditor->Render();
				break;
			}

			Event::ClearRawDelta();
			Time::Update(timer.DeltaTime());
		}
	}
};