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

	CheckNearbyCollision();

	CheckSaveStationCollision();

	scene.UpdateDirectionalLight(player->GetPosition());
		
	Event::ClearRawDelta();
}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	colliderRenderer.Render();

	terrainRenderer.Render(terrain);

	waterRenderer.Render(water);

	skeletonRenderer.Render();

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


void Game::Options()
{
	paused = true;
	currentCanvas = canvases["OPTIONS"];
}

void Game::HowToPlay()
{
	paused = true;
	currentCanvas = canvases["HOWTOPLAY"];
}

void Game::BacktoPause()
{
	paused = true;
	currentCanvas = canvases["PAUSED"];
}

void Game::MainMenu()
{
	paused = false;
	mainMenu = true;
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

	//MODELS & COLLIDERS
	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			shadowRenderer.Bind(model);
			continue;
		}
			
		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			continue;
		}

		auto boundingBox = std::dynamic_pointer_cast<BoundingBox>(drawable);
		if (boundingBox)
		{
			colliders.emplace_back(boundingBox);
			colliderRenderer.Bind(boundingBox);
			continue;
		}

		auto boundingSphere = std::dynamic_pointer_cast<BoundingSphere>(drawable);
		if (boundingSphere)
		{
			colliders.emplace_back(boundingSphere);
			colliderRenderer.Bind(boundingSphere);
			continue;
		}
	}

	for (auto& collider : colliders)
		scene.DeleteDrawable(collider->GetName());
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

void Game::AddArrow(const std::string fileName)
{
	auto arrow = std::make_shared<Arrow>(fileName);
	scene.AddModel(fileName, arrow);
	arrows.emplace_back(arrow);
	modelRenderer.Bind(scene.Get<Model>(fileName));
	shadowRenderer.Bind(scene.Get<Model>(fileName));
	arrow->SetPosition(0, -100, 0);
	arrow->SetScale(2);
	arrow->GetCollider()->SetParent(arrow);
	arrow->GetCollider()->SetScale(0.15f);
	Vector3 offset = { arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z -0.5f };
	arrow->GetCollider()->SetPosition(offset);
	colliderRenderer.Bind(arrow->GetCollider());
}

void Game::AddHostileArrow(const std::string fileName)
{
	auto arrow = std::make_shared<Arrow>(fileName);
	//scene.AddModel(fileName, arrow);
	hostileArrows.emplace_back(arrow);
	modelRenderer.Bind(arrow);
	shadowRenderer.Bind(arrow);
	arrow->SetPosition(0, -100, 0);
	arrow->SetScale(2);
	arrow->GetCollider()->SetParent(arrow);
	arrow->GetCollider()->SetScale(0.15f);
	Vector3 offset = { arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z };
	offset += {0, 0, -0.5};
	arrow->GetCollider()->SetPosition(offset);
	colliderRenderer.Bind(arrow->GetCollider());
}

void Game::CheckNearbyCollision()
{
	auto playerCollider = player->GetBounds();

	bool collided = false;
	UINT numCollided = 0;
	const UINT numColliders = 3;

	for (auto& collider : colliders)
	{
		if (numCollided >= numColliders)
			break;

		auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
		if (box)
		{
			if (Collision::Intersection(box, playerCollider))
			{
				collided = true;
				numCollided++;
			}

			continue;
		};
		
		auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
		if (sphere)
		{
			if (Collision::Intersection(sphere, playerCollider))
			{
				collided = true;
				numCollided++;
			}

			continue;
		};
	}

	if (collided)
		player->ResetToLastPosition();
}

void Game::CheckSaveStationCollision()
{
	for (auto& saveStation : saveStations)
	{
		saveStation.Update();

		if (Collision::Intersection(*saveStation.Collider(), *player->GetFrustum()))
		{
			if (Time::Get() - saveStation.LastSave() > 5 && Event::KeyIsPressed('E'))
			{
				Print("SAVED");
				player->Save("Test");
				QuestLog::Inst().Save("Test");
				saveStation.LastSave(Time::Get());
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

void Game::UnbindBuildingEffect(std::unique_ptr<BuildingEffect> effect)
{
	effect->Unbind(scene, particleRenderer);
}
  
void Game::UpdateInventoryUI()
{
	auto canvas = canvases["INGAME"];
	canvas->UpdateText("Wood", std::to_string(player->Inventory().NumOf(RESOURCE::WOOD)));
	canvas->UpdateText("Stone", std::to_string(player->Inventory().NumOf(RESOURCE::STONE)));
	canvas->UpdateText("Food", std::to_string(player->Inventory().NumOf(RESOURCE::FOOD)));
}

void TestFuncBack()
{
	Print("BACK");
}
void TestFuncResume()
{
	Print("RESUME");
}
void TestFuncOptions()
{
	Print("OPTIONS");
}
void TestFuncMenu()
{
	Print("MENU");
}


Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:deferredRenderer(clientWidth, clientHeight),
	modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED),
	colliderRenderer(DEFERRED),
	terrainRenderer(DEFERRED),
	animatedModelRenderer(DEFERRED, true),
	water(5000)
{
	//LOAD SCENE
	Initialize();

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(100, 4, 4);

	//INGAME
	auto ingameCanvas = new Canvas();
	ingameCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight }, "TestImage", "CompassBase.png");
	ingameCanvas->AddImage({ 250, 250 }, "QuestBorder", "QuestBorder.png");
	ingameCanvas->AddText({ 200, 40 }, "AC", "Active Quests", 200, 20, UI::COLOR::GRAY, UI::TEXTFORMAT::TITLE);
	ingameCanvas->AddImage({ clientWidth - 200.0f, 70 }, "Resources", "Resources.png", 0.8f);
	ingameCanvas->AddText({ clientWidth - 302.0f, 70 }, "Wood", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);
	ingameCanvas->AddText({ clientWidth - 192.0f, 70 }, "Stone", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);
	ingameCanvas->AddText({ clientWidth - 82.0f, 70 }, "Food", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);

	for (UINT i = 0; i < 10; ++i)
		ingameCanvas->AddImage({ 50.0f + 50 * i, clientHeight - 40.0f }, "hp" + std::to_string(i), "Heart.png");

	canvases["INGAME"] = ingameCanvas;
	currentCanvas = ingameCanvas;

	//PAUSED
	auto pauseCanvas = new Canvas();
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f }, "RESUME", 100, 50, UI::COLOR::GRAY, [this] { Resume(); }, TestFuncResume);
	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "Z", "Pause.png", 1.0f, 1.0f);
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.09f }, "A", 370, 133, UI::COLOR::GRAY, [this] { Resume(); }, TestFuncResume);
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.35f }, "B", 270, 100, UI::COLOR::GRAY, [this] { Options(); }, TestFuncOptions);
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.2f }, "C", 250, 100, UI::COLOR::GRAY, [this] { MainMenu(); }, TestFuncMenu);
	canvases["PAUSED"] = pauseCanvas;

	//OPTIONS
	auto optionsCanvas = new Canvas();
	optionsCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "X", "Options.png", 1.0f, 1.0f);
	optionsCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.08f }, "D", 200, 78, UI::COLOR::GRAY, [this] { Pause(); }, TestFuncResume);
	canvases["OPTIONS"] = optionsCanvas;

	for (int i = 0; i < 3; i++)
		AddArrow("Arrow");

	//PLAYER
	player = std::make_shared<Player>(file, scene.GetCamera(), canvases["INGAME"], arrows);
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
	building = std::make_shared<Building>(meshNames, materialNames, "Building", Vector3(5,5,5));

	scene.AddModel("Building", building);
	modelRenderer.Bind(building);
	shadowRenderer.Bind(building);
	scene.Get<Model>("Building")->SetPosition(10, -3, 60);
	scene.Get<Model>("Building")->SetRotation(0, -PI_DIV2, 0);

	//QUEST LOG
	questLog = std::make_unique<QuestLog>(file, player, ingameCanvas);

	//Item
	AddItem(RESOURCE::WOOD, { -10, 1, 20 });

	scene.AddFriendlyNPC("Staff");
	auto friendly = scene.Get<NPC>("Staff");
	friendly->SetPosition(40, 150, -30);
	friendly->SetScale(10);
	modelRenderer.Bind(friendly);
	shadowRenderer.Bind(friendly);

	auto particleSystem = std::make_shared<ParticleSystem>("rain.ps");
	scene.AddParticleSystem("RainingGATOS", particleSystem, Vector3{ 0,30,0 });
	particleRenderer.Bind(particleSystem);


	//ANIMATION
	auto animated = std::make_shared<AnimatedModel>("AnimatedLowPolyCharacter", "AnimatedModel");
	scene.AddDrawable("AnimatedModel", animated);
	skeletonRenderer.Bind(animated);
	animatedModelRenderer.Bind(animated);

	//SOUND
	Audio::AddAudio(L"Audio/Rain.wav");
	Audio::StartAudio();

	(void)Run();
}

Game::~Game()
{
	scene.Clear();
	Resources::Inst().Clear();

	for (auto& [name, canvas] : canvases)
		delete canvas;
}

APPSTATE Game::Run()
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

	if (mainMenu)
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed('M'))
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;
}