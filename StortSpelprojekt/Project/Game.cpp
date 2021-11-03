#include "Game.h"
#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"

void Game::Update()
{
	hovering = false;

	player->Update(terrain.GetHeightMap());

	QuestLog::Inst().Update();
	
	scene.Update();

	CheckItemCollision();

	CheckNearbyCollision();

	CheckSaveStationCollision();

	CheckQuestInteraction();

	scene.UpdateDirectionalLight(player->GetPosition());

	Event::ClearRawDelta();
}

void Game::Render()
{
	//shadowRenderer.Render();

	ShaderData::Inst().BindFrameConstants();

	Graphics::Inst().BeginFrame();

	particleRenderer.Render();

	modelRenderer.Render();

	animatedModelRenderer.Render();

	colliderRenderer.Render();

	terrainRenderer.Render(terrain);

	waterRenderer.Render(water);

	//skeletonRenderer.Render();

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
	FBXLoader meshLoader("Models");

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

std::shared_ptr<FriendlyNPC> Game::AddFriendlyNPC(const std::string fileName, Vector3 position)
{
	auto NPC = std::make_shared<FriendlyNPC>(fileName);
	NPC->SetPosition(position);

	auto collider = NPC->GetCollider();
	collider->SetParent(NPC);
	collider->Update();
	colliderRenderer.Bind(collider);

	modelRenderer.Bind(NPC);
	shadowRenderer.Bind(NPC);

	scene.AddDrawable("FriendlyNPC", NPC);

	friendlyNPCs.emplace_back(NPC);

	return NPC;
}

void Game::AddArrow(const std::string fileName)
{
	auto arrow = std::make_shared<Arrow>();
	arrows.emplace_back(arrow);
	arrow->SetPosition(0, -100, 0);
	arrow->SetScale(2);
	arrow->GetCollider()->SetParent(arrow);
	arrow->GetCollider()->SetScale(0.15);
	arrow->GetCollider()->SetPosition(arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z - 0.5f);
	modelRenderer.Bind(arrow);
	shadowRenderer.Bind(arrow);
	colliderRenderer.Bind(arrow->GetCollider());
	arrow->Update();
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

void Game::AddHostileNPC(const std::string& filename, Vector3 position, CombatStyle combatStyle)
{
	auto npc = std::make_shared<HostileNPC>(filename, player, combatStyle);
	npc->SetPosition(position);
	npc->BindPlayerArrows(arrows);
	modelRenderer.Bind(npc);
	scene.AddDrawable("hostileNpc", npc);
	npc->BindArrows(modelRenderer);

	shadowRenderer.Bind(npc);
	colliderRenderer.Bind(npc->GetCollider());
	hostiles.emplace_back(npc);
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
			hovering = true;

			if (Event::KeyIsPressed('E'))
			{
				Print("PICKED UP ITEM");
				player->Inventory().AddItem(item->GetType());
				RemoveItem(item->GetName());
				UpdateInventoryUI();
			}
		}
	}
}

void Game::CheckQuestInteraction()
{
	for (auto& NPC : friendlyNPCs)
	{
		if (NPC->Interactable())
		{
			if (Collision::Intersection(*NPC->GetCollider(), *player->GetFrustum()))
			{
				hovering = true;

				if (Event::KeyIsPressed('E'))
				{
					int ID = NPC->GetQuestID();
					if (ID != -1)
					{
						if (ID != 0)
							NPC->ConnectedBuilding()->Upgrade();

						if (ID == 4) //LAST QUEST
							done = true;

						QuestLog::Inst().Complete(ID);
					}

					return;
				}
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

	canvas->UpdateText("WOOD", std::to_string(player->Inventory().NumOf(RESOURCE::WOOD)));
	canvas->UpdateText("STONE", std::to_string(player->Inventory().NumOf(RESOURCE::STONE)));
	canvas->UpdateText("FOOD", std::to_string(player->Inventory().NumOf(RESOURCE::FOOD)));
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
	modelRenderer(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD),
	colliderRenderer(FORWARD),
	animatedModelRenderer(FORWARD, false),
	water(5000), terrain(2)
{
	//LOAD SCENE
	Initialize();

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(100, 4, 4);

	//INGAME CANVAS
	auto ingameCanvas = new Canvas();

	ingameCanvas->AddImage({ 250, 365 }, "QuestBorder", "QuestBorder.png");
	ingameCanvas->AddText({ 250, 65 }, "AQ", "Active Quests", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	const UINT offset = 162;
	for (UINT i = 0; i < ARRAYSIZE(Item::Names); ++i)
	{
		D2D_VECTOR_2F position = { (float)clientWidth - 90 - offset * i, 80.0f };
		ingameCanvas->AddImage(position, Item::Names[i] + "inventorySlot", "InventorySlot.png");
		ingameCanvas->AddImage(position, Item::Names[i] + "inventoryValue", Item::Names[i] + ".png");
		ingameCanvas->AddText({ position.x, position.y + 70 }, Item::Names[i], "0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);
	}

	ingameCanvas->AddImage({ 355, clientHeight - 64.0f }, "hp", "HP10.png");

	ingameCanvas->AddText({ (float)clientWidth - 50, (float)clientHeight - 30 }, "FPS", "0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	ingameCanvas->AddText({ (float)clientWidth / 2.0f, (float)clientHeight - 200.0f }, "INTERACT", "INTERACT [E]", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);

	canvases["INGAME"] = ingameCanvas;
	currentCanvas = ingameCanvas;

	//PAUSED CANVAS
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

	//for (int i = 0; i < 3; i++)
	//	AddHostileArrow("Arrow");

	//PLAYER
	player = std::make_shared<Player>(file, scene.GetCamera(), ingameCanvas, arrows);
	player->SetPosition(-75, 20, -630);
	scene.AddModel("Player", player);
	player->GetBounds()->SetParent(player);
	colliderRenderer.Bind(player->GetBounds());
	animatedModelRenderer.Bind(player);

	colliderRenderer.Bind(player->GetFrustum());
	player->GetFrustum()->SetParent(player);

	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNames[] = { "BuildingZero", "BuildingFirst", "BuildingSecond" };
	std::string materialNames[] = { "HouseTexture", "HouseTexture", "HouseTexture"};
	building = std::make_shared<Building>(meshNames, materialNames, "Building", Vector3{ -70, 20.5f, -566 }, scene, particleRenderer);
	building->SetRotation(0, -DirectX::XM_PIDIV2, 0);
	building->SetScale(5);

	scene.AddModel("Building", building);
	modelRenderer.Bind(building);
	shadowRenderer.Bind(building);

	//QUEST LOG
	questLog = std::make_unique<QuestLog>(file, player, ingameCanvas);

	//Item
	AddItem(WOOD, { -62, 23, -580 });

	//RAIN SYSTEM
	auto particleSystem = std::make_shared<ParticleSystem>("rain.ps");
	scene.AddParticleSystem("RainingGATOS", particleSystem, Vector3{ -70,70,-580 });
	particleRenderer.Bind(particleSystem);

	//FRIENDLY NPC
	auto friendlyNPC = AddFriendlyNPC("LowPolyCharacter", Vector3{ -70, 25.0f, -596 });
	friendlyNPC->BindBuilding(building);
	friendlyNPC->AddQuestID(0);
	friendlyNPC->AddQuestID(2);
	friendlyNPC->AddQuestID(4);
	friendlyNPC->AddQuestID(6);

	//SOUND
	//Audio::AddAudio(L"Audio/Rainy.wav");
	//Audio::StartAudio();

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

		if (Event::KeyIsPressed('Y'))
		{
			QuestLog::Inst().Complete(3);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('B'))
		{
			Print("Killed barbarian!");
			player->Stats().barbariansKilled++;
			player->TakeDamage();
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
			UpdateInventoryUI();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('R'))
		{
			building->effect->Bind(scene, particleRenderer);
			building->Upgrade();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('T'))
		{
			player->TakeDamage();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('Y'))
		{
			player->AddHealthPoint();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('P'))
		{
			player->GetStats();
			player->AddHealthPoint();
			lastClick = Time::Get();
		}

	}

	if (hovering)
		canvases["INGAME"]->GetText("INTERACT")->Show();
	else
		canvases["INGAME"]->GetText("INTERACT")->Hide();

	static float counter = 0;
	if (done)
	{
		counter += Time::GetDelta();
		if (counter >= 5.0f)
			return APPSTATE::WIN;
	}

	static int frames = 0;
	static float time = 0;

	time += Time::GetDelta();
	frames++;

	if (time >= 1.0f)
	{
		canvases["INGAME"]->UpdateText("FPS", std::to_string(frames));
		frames = 0;
		time = 0;
	}

	if (mainMenu)
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed('M'))
		return APPSTATE::MAIN_MENU;

	if (Event::KeyIsPressed('X'))
		return APPSTATE::GAMEOVER;

	if (player->GetGameOver() == true)
	{
		std::cout << "DEAD!!!" << std::endl;
		return APPSTATE::GAMEOVER;
	}

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;


	return APPSTATE::NO_CHANGE;
}

void Game::CheckNearbyEnemies()
{

	for (auto hostile : hostiles)
	{
		bool hit = player->CheckArrowHit(hostile->GetCollider());

		if (hit)
		{
			
			hostile->TakeDamage();
		}

	}



}
