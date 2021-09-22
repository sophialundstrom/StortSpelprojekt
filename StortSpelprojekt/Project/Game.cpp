#include "Game.h"
#include "Event.h"

void Game::Update()
{
	player->Update(&scene.GetCamera());


	QuestLog::Inst().Update();
	scene.Update();

}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	terrainRenderer.Render(terrain);

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
	particleRenderer(DEFERRED),
	terrainRenderer(DEFERRED), terrain(50.0f, 0)
{
	player = std::make_shared<Player>();

	scene.AddModel("Player", player);
	modelRenderer.Bind(scene.Get<Model>("Player"));
	questLog = std::make_unique<QuestLog>("Default", player);

	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, {0, 1, 0});
	scene.SetDirectionalLight(30, 4, 4);

	userInterface.Initialize(window);

	//Junk
	scene.AddModel("boulder");
	auto boulder = scene.Get<Model>("boulder");
	boulder->SetPosition(0, 5, 10);
	modelRenderer.Bind(boulder);

	(void)Run();
}

Game::~Game()
{
	scene.Clear();
	Resources::Inst().Clear();
}

State Game::Run()
{
	Update();
	Render();

	if (Event::KeyIsPressed('T'))
		QuestLog::Inst().Activate(0);

	if (Event::KeyIsPressed('U'))
		QuestLog::Inst().Complete(0);

	if (Event::KeyIsPressed('B'))
		player->GameStats().barbariansKilled++;

	if (Event::KeyIsPressed('I'))
		player->Inventory().AddItem(0);

	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}