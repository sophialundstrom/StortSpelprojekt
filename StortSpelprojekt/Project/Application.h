#pragma once
#include "Window.h"
#include "RenderGraph.h"
#include "Event.h"
#include "Time.h"
#include "Scene.h"

class Application
{
private:
	Window window;
	Scene scene;
public:
	Application(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
		:window(width, height, title, instance)
	{
		Graphics::Initialize(width, height, window.HWnd());
		RenderGraph::Initialize(width, height);
		ImGUI::Initialize();
		scene.Initialize(width, height);
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

			scene.Update();

			RenderGraph::Render();

			Event::ClearRawDelta();

			Time::Update(timer.DeltaTime());
		}
	}
};