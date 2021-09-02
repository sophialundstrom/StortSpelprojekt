#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"
#include "Scene.h"

#include "DebugMainMenu.h"
#include "DemoEditor.h"

class Application
{
private:
	Window window;
	Scene scene;

	DebugMainMenu DBMainMenu;
	DemoEditor demoEditor;

	std::unique_ptr<TempResources> resources;

	AppState state = DB_MAIN;
public:
	Application(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
		:window(width, height, title, instance)
	{
		RECT clientRect;
		GetClientRect(window.HWnd(), &clientRect);

		UINT windowWidth = clientRect.right;
		UINT windowHeight = clientRect.bottom;

		Graphics::Initialize(windowWidth, windowHeight, window.HWnd());
		RenderGraph::Initialize(windowWidth, windowHeight);
		ImGUI::Initialize();
		scene.Initialize(windowWidth, windowHeight);

		resources = std::make_unique<TempResources>();
	}

	~Application()
	{
		scene.ShutDown();
		ImGUI::ShutDown();
		RenderGraph::ShutDown();
		ShaderData::ShutDown();
		Graphics::ShutDown();
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
				scene.Update();
				RenderGraph::Render();
				break;

			case DB_LEVEL:
				demoEditor.Update();

				if (demoEditor.IsDone())
					state = DB_MAIN;

				demoEditor.Render();
				break;
			}

			Event::ClearRawDelta();
			Time::Update(timer.DeltaTime());
		}
	}
};