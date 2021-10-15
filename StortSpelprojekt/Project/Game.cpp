#include "Game.h"
#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"

void Game::Update()
{
	player->Update(terrain.GetHeightMap());

	QuestLog::Inst().Update();

	auto friendly = scene.Get<NPC>("Staff");

	friendly->Collided(*player);

	scene.Update();

	CheckItemCollision();

	CheckSaveStationCollision();

	scene.UpdateDirectionalLight(player->GetPosition());
		
	grid.CreateGrid(scene.GetDrawables());

	Event::ClearRawDelta();
}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	colliderRenderer.Render();

	terrainRenderer.Render(terrain);

	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	deferredRenderer.Render();
	
	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void Game::Pause()
{
	paused = true;
	currentCanvas = canvases["PAUSED"];
}

void Game::Resume()
{
	paused = false;
	currentCanvas = canvases["INGAME"];
}

void Game::Initialize()
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());

	//SAVE STATIONS
	saveStations[0] = SaveStation({ -20, 0, 20 }, 0, scene.GetDrawables());
	colliderRenderer.Bind(saveStations[0].Collider());

	saveStations[1] = SaveStation({ 20, 0, -20 }, 1, scene.GetDrawables());
	colliderRenderer.Bind(saveStations[1].Collider());

	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			shadowRenderer.Bind(model);
		}
			
		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}

	grid.CreateGrid(scene.GetDrawables());
}

void Game::RemoveItem(const std::string name)
{
	for (UINT i = 0; i < items.size(); ++i)
	{
		if (items[i]->GetName() == name)
		{
			auto item = scene.Get<Item>(name);
			modelRenderer.Unbind(item);
			shadowRenderer.Unbind(item);
			colliderRenderer.Unbind(item->GetBounds());
			auto it = items.begin() + i;
			items.erase(it);
			scene.DeleteDrawable(name);
			return;
		}
	}
}

void Game::AddItem(RESOURCE resource, Vector3 position)
{
	const std::string name = "testItem";

	auto item = std::make_shared<Item>(resource, name);
	scene.AddModel(name, item);
	items.emplace_back(item);
	item->GetBounds()->SetParent(item);
	item->SetPosition(position);
	item->GetBounds()->Update();
	modelRenderer.Bind(item);
	shadowRenderer.Bind(item);
	colliderRenderer.Bind(item->GetBounds());
}

void Game::CheckSaveStationCollision()
{
	for (auto& saveStation : saveStations)
	{
		saveStation.Update();

		if (Collision::Intersection(*saveStation.Collider(), *player->GetFrustum()))
		{
			if (Time::Get() - lastSave > 5 && Event::KeyIsPressed('E'))
			{
				Print("SAVED");
				player->Save("Test");
				QuestLog::Inst().Save("Test");
				lastSave = Time::Get();
			}
		}
	}
}

void Game::CheckItemCollision()
{
	for (auto &item : items)
	{
		if (Collision::Intersection(*item->GetBounds(), *player->GetFrustum()))
		{
			if (Event::KeyIsPressed('E'))
			{
				Print("PICKED UP ITEM");
				player->Inventory().AddItem(item->GetType());
				RemoveItem(item->GetName());
			}
		}
	}
}

void TestFunc()
{
	Print("HOVERING");
}

Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:deferredRenderer(clientWidth, clientHeight),
	modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED),
	terrainRenderer(DEFERRED, 40),
	colliderRenderer(DEFERRED)
{

	Initialize();

	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(50, 4, 4);

	//PLAYER
	player = std::make_shared<Player>(file, scene.GetCamera());
	scene.AddModel("Player", player);
	modelRenderer.Bind(scene.Get<Model>("Player"));
	shadowRenderer.Bind(scene.Get<Model>("Player"));
	player->GetBounds()->SetParent(player);
	colliderRenderer.Bind(player->GetBounds());
	//colliderRenderer.Bind(player->GetRay());
	colliderRenderer.Bind(player->GetFrustum());
	player->GetFrustum()->SetParent(player);

	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNames[] = { "BuildingFirst", "BuildingSecond" };
	std::string materialNames[] = { "", "HouseTexture"};
	building = std::make_shared<Building>(meshNames, materialNames, "Building");

	scene.AddModel("Building", building);
	modelRenderer.Bind(building);
	shadowRenderer.Bind(building);
	scene.Get<Model>("Building")->SetPosition(10, -3, 60);
	scene.Get<Model>("Building")->SetRotation(0, -PI_DIV2, 0);

	//UI
	userInterface = std::make_unique<UI>(window);

	//INGAME
	auto ingameCanvas = new Canvas();
	ingameCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight }, "TestImage", "CompassBase.png");
	ingameCanvas->AddImage({ 250, 250 }, "QuestBorder", "QuestBorder.png");
	ingameCanvas->AddText({ 200, 40 }, "AC", "Active Quests", 200, 20, UI::COLOR::GRAY, UI::TEXTFORMAT::TITLE);
	canvases["INGAME"] = ingameCanvas;
	currentCanvas = ingameCanvas;

	//PAUSED
	auto pauseCanvas = new Canvas();
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f }, "RESUME", 100, 50, UI::COLOR::GRAY, [this]{ Resume(); }, TestFunc);
	canvases["PAUSED"] = pauseCanvas;

	//QUEST LOG
	questLog = std::make_unique<QuestLog>(file, player, ingameCanvas);

	//Item
	AddItem(WOOD, { -10, 1, 20 });

	scene.AddFriendlyNPC("Staff");
	auto friendly = scene.Get<NPC>("Staff");

	friendly->SetPosition(10, 0, 10);
	modelRenderer.Bind(friendly);
	shadowRenderer.Bind(friendly);

	auto particleSystem = std::make_shared<ParticleSystem>("rain.ps");
	scene.AddParticleSystem("RainingGATOS", particleSystem, Vector3{ 0,30,0 });
	particleRenderer.Bind(particleSystem);

	(void)Run();
}

Game::~Game()
{
	scene.Clear();
	Resources::Inst().Clear();

	for (auto& [name, canvas] : canvases)
		delete canvas;
}

State Game::Run()
{
	if (!paused)
		Update();
	
	currentCanvas->Update();

	Render();

	static float lastClick = 0;

	if (Time::Get() - lastClick > 0.5f)
	{
		if (Event::KeyIsPressed(VK_TAB))
		{
			if (paused)
				Resume();
			else
				Pause();

			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('U'))
		{
			QuestLog::Inst().Complete(0);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('B'))
		{
			Print("Killed barbarian!");
			player->Stats().barbariansKilled++;
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('I'))
		{
			Print("-Added Items-");
			player->Inventory().AddItem(RESOURCE::WOOD);
			player->Inventory().GetResources(RESOURCE::WOOD);
			player->Inventory().AddItem(RESOURCE::STONE);
			player->Inventory().GetResources(RESOURCE::STONE);
			player->Inventory().AddItem(RESOURCE::FOOD);
			player->Inventory().GetResources(RESOURCE::FOOD);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('R'))
		{
			building->Upgrade();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('P'))
		{
			player->GetStats();
			lastClick = Time::Get();
		}
	}
	
	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}