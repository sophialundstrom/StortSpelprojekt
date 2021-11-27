#include "Game.h"
#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "DialogueOverlay.h"

void Game::Update()
{
	hovering = false;

	QuestLog::Inst().Update();

	scene.Update();

	player->Update(terrain.GetHeightMap(), modelRenderer, colliderRenderer);

	scene.GetCamera()->Update();

	CheckItemCollision();

	CheckNearbyCollision();

	CheckSaveStationCollision();

	CheckNearbyEnemies();

	CheckQuestInteraction();

	UpdateAndHandleLoot();

	scene.UpdateDirectionalLight(player->GetPosition());

	UpdateQuadTree(); //Something in here makes the game run twice as fast

	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), 0, nullptr);

	Event::ClearRawDelta();
}

void Game::Render()
{
	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	//particleRenderer.Render();

	//modelRenderer.Render();

	//staticMeshModelRender.Render();

	//animatedModelRenderer.Render();

	//colliderRenderer.Render();

	terrainRenderer.Render(terrain);

	//waterRenderer.Render(water);

	//skeletonRenderer.Render();----

	skyBoxRenderer.Render();
	
	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void Game::Pause()
{
	state = GameState::PAUSED;
	currentCanvas = canvases["PAUSED"];
}

void Game::Resume()
{
	state = GameState::ACTIVE;
	currentCanvas = canvases["INGAME"];
}

void Game::Options()
{
	currentCanvas = canvases["OPTIONS"];
}

void Game::HowToPlay()
{
	currentCanvas = canvases["HOW TO PLAY"];
}

void Game::BacktoPause()
{
	currentCanvas = canvases["PAUSED"];
}

void Game::MainMenu()
{
	mainMenu = true;
}

void Game::Initialize()
{
	QuadTreeBounds qtBounds(-1000.f, -1000.f, 2000.f, 2000.f);
	quadTree = new QuadTree(qtBounds, 4, 5, 0, "Master");
	

	//LOAD SCENE
	FBXLoader meshLoader("Models");
	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());
	

	
	//Transfer drawables to quadTree
	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if(model)
			quadTree->InsertModel(drawable);
	}
	
	//For Future work do not remove atm:
	/*quadTree->GetAllDrawables(noCullingDrawables);
	//quadTree->OptimizeBounds();
	quadTree->PrintTree();*/

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

			//modelRenderer.Bind(model);
			//shadowRenderer.Bind(model);

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
	scene.AddDrawable(name, item);
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

	auto marker = NPC->GetQuestMarker();
	marker->SetParent(NPC);
	modelRenderer.Bind(marker);

	return NPC;
}

void Game::AddArrow(const std::string fileName)
{
	auto arrow = std::make_shared<Arrow>();
	arrows.emplace_back(arrow);
	arrow->SetPosition(0, -100, 0);
	arrow->SetScale(2);
	//arrow->GetCollider()->SetParent(arrow);
	//arrow->GetCollider()->SetScale(0.15f);
	//arrow->GetCollider()->SetPosition(arrow->GetCollider()->GetPosition().x, arrow->GetCollider()->GetPosition().y, arrow->GetCollider()->GetPosition().z - 0.5f);
	modelRenderer.Bind(arrow);
	shadowRenderer.Bind(arrow);
	//colliderRenderer.Bind(arrow->GetCollider());
	arrow->Update();
}

void Game::UpdateAndHandleLoot()
{
	for (int i = 0; i < loot.size(); i++)
	{
		loot[i]->Update(player);
		if (loot[i]->IsDestroyed())
		{
			scene.DeleteDrawable(loot[i]->GetName());
			modelRenderer.Unbind(loot[i]);
			colliderRenderer.Unbind(loot[i]->GetCollider());
			loot[i] = std::move(loot[loot.size() - 1]);
			loot.resize(loot.size() - 1);
			SoundEffect::AddAudio(L"Audio/PickupPop.wav", 2);
			SoundEffect::SetVolume(0.1, 2);
			SoundEffect::StartAudio(2);
			//std::cout << "Loot destoyed\n";
		}
	}
}

void Game::CheckNearbyCollision()
{
	auto playerCollider = player->GetBounds();

	bool collided = false;

	float closestCamCollision = 9999;
	
	std::vector<std::shared_ptr<Collider>> collidedColliders;

	for (auto& collider : colliders)
	{
		for (auto& hostile : hostiles)
		{
			for (auto& arrow : hostile->GetArrowHandler().arrows)
			{
				if (!arrow->canCollide)
					continue;
				hostile->GetArrowHandler().CheckCollision(arrow, collider);
			}
		}

		
		for (auto& arrow : player->GetArrowHandler().arrows)
		{
			if (!arrow->canCollide)
				continue;

			player->GetArrowHandler().CheckCollision(arrow, collider);
		}

		auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
		if (box)
		{

			if (Collision::Intersection(box, playerCollider))
			{
				collided = true;
				collidedColliders.emplace_back(box);
			}
				

			Collision::RayResults colliderResult = Collision::Intersection(*box, *scene.GetCamera()->GetCamRay());
			
			if (colliderResult.didHit)
			{
				if (colliderResult.distance < closestCamCollision)
					closestCamCollision = colliderResult.distance;
			}

			continue;
		};

		auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
		if (sphere)
		{
			if (Collision::Intersection(sphere, playerCollider))
			{
				collided = true;
				collidedColliders.emplace_back(sphere);
			}

			Collision::RayResults colliderResult = Collision::Intersection(*sphere, *scene.GetCamera()->GetCamRay());
			if (colliderResult.didHit)
			{
				if (colliderResult.distance < closestCamCollision)
					closestCamCollision = colliderResult.distance;
			}

			continue;
		};
	}

	player->SetClosestColliderToCam(closestCamCollision);

	player->HandleCollidedObjects(collidedColliders);
}

void Game::AddHostileNPC(const std::string& filename, Vector3 position)
{
	auto NPC = std::make_shared<HostileNPC>(filename, player, modelRenderer, colliderRenderer);
	NPC->SetPosition(position);
	//NPC->BindArrows(modelRenderer);

	auto collider = NPC->GetCollider();
	collider->SetParent(NPC);
	collider->Update();
	collider->SetScale(2, 7, 2);
	colliderRenderer.Bind(collider);

	modelRenderer.Bind(NPC);
	shadowRenderer.Bind(NPC);
	const std::string name = "hostileNPC" + std::to_string(hostileID);
	scene.AddDrawable(name, NPC);
	hostileID++;
	hostiles.emplace_back(NPC);
}

void Game::AddLoot(LOOTTYPE type, const Vector3& position)
{

	auto LOOT = std::make_shared<Loot>(type, position);
	modelRenderer.Bind(LOOT);
	auto collider = LOOT->GetCollider();
	const std::string name = "loot" + std::to_string(lootID);
	LOOT->SetName(name);
	scene.AddDrawable(name, LOOT);
	loot.emplace_back(LOOT);
	lootID++;
	//colliderRenderer.Bind(LOOT->GetCollider());
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
	for (auto& item : items)
	{
		if (Collision::Intersection(*item->GetBounds(), *player->GetFrustum()))
		{
			hovering = true;

			if (Event::KeyIsPressed('E'))
			{
				SoundEffect::AddAudio(L"Audio/Pickup.wav", 2);
				SoundEffect::SetVolume(0.005, 2);
				SoundEffect::StartAudio(2);
				Print("PICKED UP ITEM");
				player->Inventory().AddItem(item->GetType());
				RemoveItem(item->GetName());
				UpdateInventoryUI();
				return;
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
					state = GameState::DIALOGUE;
					SoundEffect::AddAudio(L"Audio/Welcome.wav", 2);
					SoundEffect::SetVolume(0.004, 2);
					SoundEffect::StartAudio(2);
					auto dialogueOverlay = std::dynamic_pointer_cast<DialogueOverlay>(canvases["DIALOGUE"]);
					dialogueOverlay->Set("GILBERT", "Lorem.");
					currentCanvas = dialogueOverlay;

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
	auto& canvas = canvases["INGAME"];

	canvas->UpdateText("WOOD", std::to_string(player->Inventory().NumOf(RESOURCE::WOOD)));
	canvas->UpdateText("STONE", std::to_string(player->Inventory().NumOf(RESOURCE::STONE)));
	canvas->UpdateText("FOOD", std::to_string(player->Inventory().NumOf(RESOURCE::FOOD)));
}

Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, true),
	staticMeshModelRender(FORWARD, true),
	particleRenderer(FORWARD),
	terrainRenderer(FORWARD),
	colliderRenderer(FORWARD),
	animatedModelRenderer(FORWARD, true),
	water(5000), terrain(2)
{
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, { 1, 1, 1, 1 }, 4, 4);
	//LOAD SCENE
	Initialize();



	//INGAME CANVAS
	auto ingameCanvas = std::make_shared<Canvas>();
	ingameCanvas->HideCursor();
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

	ingameCanvas->AddImage({ (float)clientWidth / 2.0f, (float)clientHeight / 2 }, "CrossHair", "CrossHair.png");
	

	ingameCanvas->AddText({ (float)clientWidth / 2.0f, (float)clientHeight - 50 }, "ArrowCount", "Arrows:" + std::to_string(0), UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	canvases["INGAME"] = ingameCanvas;
	currentCanvas = ingameCanvas;

	//PAUSED CANVAS
	auto pauseCanvas = std::make_shared<Canvas>();
	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "PauseBackground", "PauseBackground.png", 1.0f, 1.0f);
	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 8.0f }, "PauseTitle", "PAUSED.png", 1.0f, 1.0f);

	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f - 100 }, "RESUME", 350, 95, UI::COLOR::GRAY, [this] { Resume(); });
	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f - 100}, "ResumeButton", "ResumeButton.png", 0.50f, 1.0f);

	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "HowToPlayButton", "HowToPlayButton.png", 0.50f, 1.0f);
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f }, "HowToPlay", 350, 95, UI::COLOR::GRAY, [this] { HowToPlay(); });

	pauseCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f + 100 }, "BackToMainMenu", "MainMenuButton.png", 0.50f, 1.0f);
	pauseCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 2.0f + 100}, "BackToMainMenuButton", 350, 95, UI::COLOR::GRAY, [this] { MainMenu(); });

	canvases["PAUSED"] = pauseCanvas;

	//HOW TO PLAY
	auto howToPlayCanvas = std::make_shared<Canvas>();
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 2.0f }, "ControlImage", "Controls.png", 2.0f, 1.0f);
	howToPlayCanvas->AddImage({ clientWidth / 2.0f, clientHeight / 1.1f }, "BackHowToPlay", "BackButton.png", 0.5f, 1.0f);
	howToPlayCanvas->AddButton({ clientWidth / 2.0f, clientHeight / 1.1f }, "BackButtonHowToPlay", 340, 90, UI::COLOR::GRAY, [this] { BacktoPause(); });

	canvases["HOW TO PLAY"] = howToPlayCanvas;

	canvases["DIALOGUE"] = std::make_unique<DialogueOverlay>();


	// THE WILL BE A PROBLEM IF MORE ARROWS THAN MAXARROWS IS IN THE AIR AT THE SAME TIME (NO ARROW WILL BE RENDERED). THIS IS BECAUSE THERE ARE ONLY AS MANY ARROW MODELS AS MAXARROWS.

	//PLAYER
	UINT maxArrows = 5;
	player = std::make_shared<Player>(file, scene.GetCamera(), ingameCanvas, maxArrows);
	player->SetPosition(-75, 20, -630);
	//scene.AddModel("Player", player);
	player->GetBounds()->SetParent(player);
	colliderRenderer.Bind(player->GetBounds());
	animatedModelRenderer.Bind(player);

	colliderRenderer.Bind(player->GetFrustum());
	player->GetFrustum()->SetParent(player);

	colliderRenderer.Bind(scene.GetCamera()->GetCamRay());
	
	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNames[] = { "BuildingZero", "BuildingFirst", "BuildingSecond" };
	std::string materialNames[] = { "FarmHouse", "FarmHouse", "FarmHouse" };
	building = std::make_shared<Building>(meshNames, materialNames, "Building", Vector3{ -107.5f, 20.0f, -608.5f }, scene, particleRenderer);
	building->SetRotation(0, -DirectX::XM_PI, 0);
	building->SetScale(5.85);

	scene.AddModel("Building", building);
	modelRenderer.Bind(building);
	shadowRenderer.Bind(building);

	//QUEST LOG
	questLog = std::make_unique<QuestLog>(file, player, ingameCanvas);

	//Item
	AddItem(WOOD, { -134, 22, -594 });
	AddItem(WOOD, { -113, 22, -582 });
	AddItem(WOOD, { -116, 20, -609 });
	AddItem(WOOD, { -91, 20, -593 });
	AddItem(WOOD, { -85, 20, -608 });

	AddHostileNPC("BarbarianBow", { Vector3(5.686, 20, -592.456) + Vector3(0,6,0) });
	//AddHostileNPC("BarbarianBow", { Vector3(0, 20, -592.456) + Vector3(0,6,0) }, CombatStyle::consistantDelay);
	//AddHostileNPC("BarbarianBow", { player->GetPosition() + Vector3(15,6,0) }, CombatStyle::consistantDelay);
	//AddHostileNPC("BarbarianBow", { 120, 24, -700 }, CombatStyle::consistantDelay);


	//FRIENDLY NPC
	auto friendlyNPC = AddFriendlyNPC("Priest", Vector3{ -70.0f, 20.0f, -596.0f });
	friendlyNPC->BindBuilding(building);
	friendlyNPC->AddQuestID(0);
	friendlyNPC->AddQuestID(2);
	friendlyNPC->AddQuestID(4);
	friendlyNPC->AddQuestID(6);

	auto campFireSystem = std::make_shared<ParticleSystem>("fire.ps");
	scene.AddParticleSystem("CampfireSystem", campFireSystem, Vector3{ 38.0f, 20.3f, -574.5f });
	particleRenderer.Bind(campFireSystem);
	
	Audio::AddAudio(L"Audio/Sonrie.wav", 0);
	Audio::SetVolume(0.005, 0);
	//Audio::SetVolume(0.1, 0);
	Audio::StartAudio(0);
	
	(void)Run();
}

Game::~Game()
{
	delete quadTree;
	scene.Clear();
	Resources::Inst().Clear();
}

APPSTATE Game::Run()
{
	if (state != GameState::PAUSED)
		Update();

	currentCanvas->Update();

	if (state == GameState::DIALOGUE)
	{
		auto overlay = std::dynamic_pointer_cast<DialogueOverlay>(canvases["DIALOGUE"]);
		if (overlay->IsDone())
		{
			state = GameState::ACTIVE;
			currentCanvas = canvases["INGAME"];
		}
	}

	Render();

	static float lastClick = 0;

	if (Time::Get() - lastClick > 0.5f)
	{
		if (Event::KeyIsPressed(VK_TAB))
		{
			if (state == GameState::PAUSED)
				Resume();
			else
				Pause();

			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed(VK_RETURN))
		{
			AddHostileNPC("BarbarianBow", { player->GetPosition() + Vector3(0,6,0) });
			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed('1'))
		{
			Graphics::Inst().ActivateWireframe();
			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed('2'))
		{
			Graphics::Inst().DeactivateWireframe();
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('V'))
		{
			PrintNumber("Player Arrows", player->GetArrowHandler().arrows.size());
			PrintNumber("Barb Arrows", hostiles[0]->GetArrowHandler().arrows.size());
			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed('K'))
		{
			Audio::AddAudio(L"Audio/arrowHit.wav", 0);
			Audio::SetVolume(0.3, 0);
			Audio::StartAudio(0);
			hostiles[0]->TakeDamage();
			player->Stats().barbariansKilled++;
			hostiles[0]->GetArrowHandler().ClearArrows(modelRenderer, colliderRenderer);
			colliderRenderer.Unbind(hostiles[0]->GetCollider());
			modelRenderer.Unbind(hostiles[0]);
			scene.DeleteDrawable(hostiles[0]->GetName());
			hostiles[0] = hostiles[hostiles.size() - 1];
			hostiles.resize(hostiles.size() - 1);
			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed(79))
		{
			Audio::StopEngine();
		}

		/*if (Event::KeyIsPressed('U'))
		{
			QuestLog::Inst().Complete(0);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed('Y'))
		{
			QuestLog::Inst().Complete(3);
			lastClick = Time::Get();
		}*/

		/*if (Event::KeyIsPressed('B'))
		{
			Print("Killed barbarian!");
			player->Stats().barbariansKilled++;
			player->TakeDamage();
			lastClick = Time::Get();
		}*/

		/*	if (Event::KeyIsPressed('I'))
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
		}*/

		/*if (Event::KeyIsPressed('R'))
		{
			building->effect->Bind(scene, particleRenderer);
			building->Upgrade();
			lastClick = Time::Get();
		}*/

	}

	UpdateInventoryUI();

	canvases["INGAME"]->UpdateText("ArrowCount", "Arrows: " + std::to_string(player->numArrows));
	
	if (hovering)
		canvases["INGAME"]->GetText("INTERACT")->Show();
	else
		canvases["INGAME"]->GetText("INTERACT")->Hide();

	if (Event::RightIsClicked())
		canvases["INGAME"]->GetImage("CrossHair")->Show();
	else
		canvases["INGAME"]->GetImage("CrossHair")->Hide();

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

	if (player->GetGameOver() == true)
	{
		return APPSTATE::GAMEOVER;
	}

	if (Event::KeyIsPressed('X'))
	{
		return APPSTATE::EXIT;
	}
	return APPSTATE::NO_CHANGE;
}

void Game::CheckNearbyEnemies()
{
	for (int i = 0; i < hostiles.size(); i++)
	{
		
		hostiles[i]->Update(modelRenderer, colliderRenderer, player);

		hostiles[i]->CheckPlayerCollision(player);

		for (auto& arrow : player->GetArrowHandler().arrows)
		{
			if (!arrow->canCollide)
				continue;

			bool isDead = false;
			// CRASHES HERE IF TWO ARROWS HIT IT IN THE SAME FRAME...
			bool hit = player->GetArrowHandler().CheckCollision(arrow, hostiles[i]->GetCollider(), true);

			if (hit)
			{
				std::cout << "BARBARIAN " << i << " HIT!" << std::endl;
				SoundEffect::AddAudio(L"Audio/BarbarianHit.wav", 2);
				SoundEffect::SetVolume(0.5, 2);
				SoundEffect::StartAudio(2);
				hostiles[i]->TakeDamage();
				if (hostiles[i]->IsDead())
				{
					SoundEffect::AddAudio(L"Audio/Scream.wav", 2);
					SoundEffect::SetVolume(0.8, 2);
					SoundEffect::StartAudio(2);
					hostiles[i]->TakeDamage();
					player->Stats().barbariansKilled++;
					AddLoot(LOOTTYPE::ARROWS, hostiles[i]->GetPosition() + Vector3(0, -3, 0));
					hostiles[i]->GetArrowHandler().ClearArrows(modelRenderer, colliderRenderer);
					colliderRenderer.Unbind(hostiles[i]->GetCollider());
					modelRenderer.Unbind(hostiles[i]);
					scene.DeleteDrawable(hostiles[i]->GetName());
					hostiles[i] = hostiles[hostiles.size() - 1];
					hostiles.resize(hostiles.size() - 1);
					isDead = true;
				}
			}
			if (isDead)
				break;
		}
	}
}

void Game::UpdateQuadTree()
{
	drawablesToBeRendered.clear();
	staticMeshModelRender.Clear();
	shadowRenderer.ClearStatic();

	frustrumCollider.Update(scene.GetCamera());
	quadTree->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);

	for (auto& [name, drawable] : drawablesToBeRendered)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			staticMeshModelRender.Bind(drawable);
		}
	}
	//std::cout << "Meshes drawn " << drawablesToBeRendered.size() << std::endl;

	orthographicCollider.Update(scene.GetDirectionalLight());
	quadTree->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);

	for (auto& [name, drawable] : drawablesToBeRendered)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			shadowRenderer.BindStatic(drawable);
		}
	}
	//std::cout << "Shadows drawn " << drawablesToBeRendered.size() << std::endl << std::endl;
	

	
	//DebugVariant
	/*
	int click;
	static float lastClick = 0;
	if (Time::Get() - lastClick > 0.5f)
	{
		if (Event::KeyIsPressed('K'))
		{
			lastClick = Time::Get();
			cullingProfile++;
			cullingProfile = cullingProfile % 3;

			switch (cullingProfile)
			{
			case 0:
				std::cout << "Use culling\nFrustrum update \n";
				useQuadTreeCulling = true;
				updateFrustrum = true;
				break;

			case 1:
				std::cout << "Use culling\nNo frustrum update \n";
				useQuadTreeCulling = true;
				updateFrustrum = false;
				break;

			case 2:
				std::cout << "No culling\n";
				useQuadTreeCulling = false;
				updateFrustrum = false;
				break;
			default:
				break;
			}
			std::cout << "Culling PROFILE: " << cullingProfile << std::endl;
		}

		if (Event::KeyIsPressed('L'))
		{
			lastClick = Time::Get();
			std::cout << drawablesToBeRendered.size() << std::endl;
		}

	}

	if(updateFrustrum)
		frustrumCollider.Update(scene.GetCamera());

	drawablesToBeRendered.clear();
	quadTree->GetRelevantDrawables(drawablesToBeRendered, frustrumCollider);

	if (useQuadTreeCulling)
	{
		staticMeshModelRender.Clear();
		for (auto& [name, drawable] : drawablesToBeRendered)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (model)
				staticMeshModelRender.Bind(drawable);
		}

	}
	else
	{
		staticMeshModelRender.Clear();
		for (auto& [name, drawable] : noCullingDrawables)
		{
			//std::cout << noCullingDrawables.size() << std::endl;
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (model)
				staticMeshModelRender.Bind(drawable);
		}

	}
	*/


}
