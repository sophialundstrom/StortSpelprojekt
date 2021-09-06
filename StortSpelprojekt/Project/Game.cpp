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

	deferredRenderer.Render();
}

Game::Game(UINT clientWidth, UINT clientHeight)
	:deferredRenderer(clientWidth, clientHeight)
{
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 5.0f, -10.0f });
	scene.SetDirectionalLight(30);

	//TO DO: LOAD SCENE

	(void)Run();
}

Game::~Game()
{
	scene.Clear();
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