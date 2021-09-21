#include "Game.h"
#include "Event.h"

void Game::Update()
{
	scene.Update();
}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	deferredRenderer.Render();
	
	//RENDER UI PROBABLY
	userInterface.Render();

	Graphics::Inst().EndFrame();


}

Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:deferredRenderer(clientWidth, clientHeight), 
	modelRenderer(DEFERRED, true), 
	particleRenderer(DEFERRED)
{
	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 5.0f, -10.0f });
	scene.SetDirectionalLight(30);

	deferredRenderer.SetRenderTargets();
	userInterface.Initialize(window);
	(void)Run();
}

Game::~Game()
{
	scene.Clear();
	Resources::Inst().Clear();
	std::cout << "Game Deleted\n";
}

State Game::Run()
{
	Update();
	Render();

	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}