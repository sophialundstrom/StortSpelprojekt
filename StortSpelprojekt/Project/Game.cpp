#include "Game.h"
#include "Event.h"

void Game::Update()
{
	scene.Update();

	//scene.GetCamera() Do whatever with this

	//TO DO: FIGURE OUT A NICE MOVEMENT IN EDITOR
	if (Event::KeyIsPressed('Q'))
		scene.GetCamera().Rotate(0, 1);

	if (Event::KeyIsPressed('E'))
		scene.GetCamera().Rotate(0, -1);

	if (Event::KeyIsPressed('W'))
		scene.GetCamera().MoveForward();

	if (Event::KeyIsPressed('A'))
		scene.GetCamera().MoveRight(-1);

	if (Event::KeyIsPressed('S'))
		scene.GetCamera().MoveForward(-1);

	if (Event::KeyIsPressed('D'))
		scene.GetCamera().MoveRight();

	if (Event::KeyIsPressed(32)) //SPACE
		scene.GetCamera().MoveUp();

	if (Event::KeyIsPressed(16)) //SHIFT
		scene.GetCamera().MoveUp(-1);

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

	Graphics::Inst().EndFrame();
}

Game::Game(UINT clientWidth, UINT clientHeight)
	:deferredRenderer(clientWidth, clientHeight), 
	modelRenderer(DEFERRED, true), 
	particleRenderer(DEFERRED)
{
	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 5.0f, -10.0f });
	scene.SetDirectionalLight(30);


	//Player
	scene.AddModel("PlayerArrow");
	scene.Get<Model>("PlayerArrow")->SetPosition(0, 0, 0);
	modelRenderer.Bind(scene.Get<Model>("PlayerArrow"));



	deferredRenderer.SetRenderTargets();
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