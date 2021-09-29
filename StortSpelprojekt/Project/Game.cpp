#include "Game.h"
#include "Event.h"

void Game::Update()
{
	player->Update(terrain.GetHeightMap());

	QuestLog::Inst().Update();

	//ROTATING BOULDER AROUND PLAYER
	auto boulder = scene.Get<Model>("boulder");
	//const Vector3 newPosition = boulder->GetPosition() * boulder->GetRotation() * -boulder->GetPosition();
	//boulder->SetPosition(newPosition);
	boulder->SetRotation(0, boulder->GetRotation().y + 0.001, 0);

	scene.Update();

	Event::ClearRawDelta();
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
	
	userInterface.Render();

	Graphics::Inst().EndFrame();
}

Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:deferredRenderer(clientWidth, clientHeight), 
	modelRenderer(DEFERRED, true), 
	particleRenderer(DEFERRED),
	terrainRenderer(DEFERRED), terrain(50.0f, 0)
{
	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 1.0f, 20.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(30, 4, 4);

	//PLAYER
	player = std::make_shared<Player>(scene.GetCamera());
	scene.AddModel("Player", player);
	modelRenderer.Bind(scene.Get<Model>("Player"));

	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNames[] = { "House", "Cube", "pSphere1" };
	std::string materialNames[] = { "HouseTex", "WaterTex", "phong1" };
	building = std::make_shared<Building>(meshNames, materialNames, "House");
	scene.AddModel("Building", building);
	modelRenderer.Bind(scene.Get<Model>("Building"));
	scene.Get<Model>("Building")->SetPosition(10, 25, 10);


	//QUEST LOG
	questLog = std::make_unique<QuestLog>("Default", player);

	//UI
	userInterface.Initialize(window);

	//FILLERS
	scene.AddModel("boulder");
	auto boulder = scene.Get<Model>("boulder");
	boulder->SetParent(scene.Get<Model>("Player"));
	boulder->SetPosition(5, 2, 0);
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

	static float lastClick = 0;

	if (Time::Get() - lastClick > 0.25f)
	{
		if (Event::KeyIsPressed('U'))
		{
			QuestLog::Inst().Complete(0);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('B'))
		{
			player->GameStats().barbariansKilled++;
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('I'))
		{
			player->Inventory().AddItem(0);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('R'))
		{
			building->Upgrade();
			lastClick = Time::Get();
		}
	}
	
	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}