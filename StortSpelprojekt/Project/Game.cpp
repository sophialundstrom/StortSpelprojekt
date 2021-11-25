#include "Game.h"
#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "DialogueOverlay.h"

void Game::Update()
{
	hovering = false;
	
	scene.Update();

	if (overlay != dialogueOverlay)
		player->Update(terrain.GetHeightMap());

	scene.GetCamera()->Update();

	CheckItemCollision();

	CheckNearbyCollision();

	CheckSaveStationCollision();

	CheckNearbyEnemies();

	CheckTargetCollision();

	CheckQuestInteraction();

	UpdateAndHandleLoot();

	scene.UpdateDirectionalLight(player->GetPosition());

	QuestLog::Update(player, camps, targets);

	ingameOverlay->UpdateArrowCounter(player->numArrows);
	ingameOverlay->UpdateHealth(player->Stats().healthPoints);
	ingameOverlay->UpdateInventory(player->Inventory());
	ingameOverlay->UpdateQuests(QuestLog::GetActiveQuests());
	UpdateQuadTree(); //Something in here makes the game run twice as fast

	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), 0, nullptr);

	Event::ClearRawDelta();
}

void Game::Render()
{
	SR->Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	PR->Render();

	MR->Render();

	AMR->Render();

	SKR->Render();
	
	staticMeshModelRender.Render();

	animatedModelRenderer.Render();

	//CR->Render();

	IR->Render();

	TR->Render(terrain);

	WR->Render(water);

	overlay->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void Game::Pause()
{
	state = GameState::PAUSED;
	currentCanvas = canvases["PAUSED"];
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
	CR->Bind(saveStations[0].Collider());

	saveStations[1] = SaveStation({ 20, 0, -20 }, 1, scene.GetDrawables());
	CR->Bind(saveStations[1].Collider());

	//MODELS & COLLIDERS
	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			MR->Bind(model);
			SR->Bind(model);

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
			CR->Bind(boundingBox);
			continue;
		}

		auto boundingSphere = std::dynamic_pointer_cast<BoundingSphere>(drawable);
		if (boundingSphere)
		{
			colliders.emplace_back(boundingSphere);
			CR->Bind(boundingSphere);
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
			MR->Unbind(item);
			SR->Unbind(item);
			CR->Unbind(item->GetCollider());
			auto it = items.begin() + i;
			items.erase(it);
			scene.DeleteDrawable(name);
			return;
		}
	}
}

void Game::AddItem(Item::Type type, Vector3 position)
{
	const std::string name = "Item";

	auto item = std::make_shared<Item>(type, name);
	auto collider = item->GetCollider();

	scene.AddDrawable(name, item);
	items.emplace_back(item);
	MR->Bind(item);
	SR->Bind(item);
	CR->Bind(item->GetCollider());
}

std::shared_ptr<FriendlyNPC> Game::AddFriendlyNPC(const std::string& name, const std::string& fileName, Vector3 position)
{
	auto NPC = std::make_shared<FriendlyNPC>(name, fileName);
	NPC->SetPosition(position);

	auto collider = NPC->GetCollider();
	collider->SetParent(NPC);
	collider->Update();
	CR->Bind(collider);

	MR->Bind(NPC);
	SR->Bind(NPC);

	scene.AddDrawable(name, NPC);

	friendlyNPCs.emplace_back(NPC);

	auto marker = NPC->GetQuestMarker();
	marker->SetParent(NPC);
	MR->Bind(marker);

	return NPC;
}

void Game::UpdateAndHandleLoot()
{
	for (int i = 0; i < loot.size(); i++)
	{
		loot[i]->Update(player);
		if (loot[i]->IsDestroyed())
		{
			scene.DeleteDrawable(loot[i]->GetName());
			MR->Unbind(loot[i]);
			CR->Unbind(loot[i]->GetCollider());
			loot[i] = std::move(loot[loot.size() - 1]);
			loot.resize(loot.size() - 1);
			SoundEffect::AddAudio(L"Audio/PickupPop.wav", 2);
			SoundEffect::SetVolume(0.1, 2);
			SoundEffect::StartAudio(2);
			//std::cout << "Loot destoyed\n";
		}
	}
}

void Game::AddFriendlyNPCs()
{
	//NPC1
	{
		auto NPC = AddFriendlyNPC("Gilbert", "Priest", { -134, 25, -594 });

		{
			auto quest = NPC->AddQuest("A Helping Hand.");
			NPC->AddDialogue("A Helping Hand. >> INSERT DIALOGUE FOR HANDING OUT THE QUEST");
			NPC->AddDialogue("A Helping Hand. >> INSERT DIALOGUE FOR GETTING HELP DURING QUEST");
			NPC->AddDialogue("A Helping Hand. >> INSERT DIALOGUE FOR HANDING IN THE QUEST");

			// EXISTS ON ACTIVATE/UNLOCKED/COMPLETE SAME THING JUST DIFFERENT NAMES

			auto onCompleteFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};

			quest->AddOnCompleteFunction(onCompleteFunc);
		}

		{
			auto quest = NPC->AddQuest("Target Aquired.");
			NPC->AddDialogue("Target Aquired. >> INSERT DIALOGUE FOR HANDING OUT THE QUEST");
			NPC->AddDialogue("Target Aquired. >> INSERT DIALOGUE FOR GETTING HELP DURING QUEST");
			NPC->AddDialogue("Target Aquired. >> INSERT DIALOGUE FOR HANDING IN THE QUEST");

			auto onCompleteFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};

			quest->AddOnCompleteFunction(onCompleteFunc);
		}

		{
			auto quest = NPC->AddQuest("We're Under Attack!");
			NPC->AddDialogue("We're Under Attack! >> INSERT DIALOGUE FOR HANDING OUT THE QUEST");
			NPC->AddDialogue("We're Under Attack! >> INSERT DIALOGUE FOR GETTING HELP DURING QUEST");
			NPC->AddDialogue("We're Under Attack! >> INSERT DIALOGUE FOR HANDING IN THE QUEST");

			auto onCompleteFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};

			quest->AddOnCompleteFunction(onCompleteFunc);
		}

		NPC->AddDialogue("INSERT DIALOGUE FOR WHEN NPC HAS NO QUESTS LEFT");

		friendlyNPCs.emplace_back(NPC);
	}

	//NPC2
	{
		auto NPC = AddFriendlyNPC("Gilbert2", "Priest", { -144, 25, -594 });

		{
			Quest* quest = NPC->AddQuest("FIRST QUEST FOR NPC2");
			NPC->AddDialogue("FIRST QUEST FOR NPC2 >> INSERT DIALOGUE FOR HANDING OUT THE QUEST");
			NPC->AddDialogue("FIRST QUEST FOR NPC2 >> INSERT DIALOGUE FOR GETTING HELP DURING QUEST");
			NPC->AddDialogue("FIRST QUEST FOR NPC2 >> INSERT DIALOGUE FOR HANDING IN THE QUEST");

			auto onActivateFunc = [this, quest]() mutable
			{
				for (auto& FNPC : friendlyNPCs)
				{
					if (FNPC->GetName() == "Gilbert3")
					{
						FNPC->ApplyDialogueOverride();
						break;
					}
				}
			};

			quest->AddOnActivateFunction(onActivateFunc);
		}

		NPC->AddDialogue("INSERT DIALOGUE FOR WHEN NPC HAS NO QUESTS LEFT");

		friendlyNPCs.emplace_back(NPC);
	}

	//NPC3
	{
		auto NPC = AddFriendlyNPC("Gilbert3", "Priest", { -154, 25, -594 });

		NPC->AddQuest("FIRST QUEST FOR NPC3");
		NPC->AddDialogue("FIRST QUEST FOR NPC3 >> INSERT DIALOGUE FOR HANDING OUT THE QUEST");
		NPC->AddDialogue("FIRST QUEST FOR NPC3 >> INSERT DIALOGUE FOR GETTING HELP DURING QUEST");
		NPC->AddDialogue("FIRST QUEST FOR NPC3 >> INSERT DIALOGUE FOR HANDING IN THE QUEST");

		NPC->AddDialogue("INSERT DIALOGUE FOR WHEN NPC HAS NO QUESTS LEFT");

		friendlyNPCs.emplace_back(NPC);
	}

	//NPC4
}

void Game::AddTarget(const std::string& file, const Vector3& position, const Vector3& rotation)
{
	auto target = std::make_shared<Target>(file, position, rotation, targets.size());
	MR->Bind(target);
	SR->Bind(target); 
	//IR->Bind(target);

	auto collider = target->GetCollider();
	colliders.emplace_back(collider);
	CR->Bind(collider);

	targets.emplace_back(target);
}

void Game::AddBarbarianCamps()
{
	const float towerHeight = 20.0f;

	{ // SOUTHERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, BarbarianCamp::Location::South, 30.0f);
		//camp->AddBarbarian("BarbarianBow", { 392, 182, -44 }, hostiles, player, CombatStyle::consistantDelay, false);
		camp->AddBarbarian("BarbarianBow", { 120, 24, -700 }, hostiles, player, CombatStyle::consistantDelay, false);
		//ADD MORE BARBARIANS AND CAMPS

		camps[BarbarianCamp::Location::South] = camp;
	}

	{ // EASTERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, BarbarianCamp::Location::East, 40.0f);

		camps[BarbarianCamp::Location::East] = camp;
	}

	{ // NORTHERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, BarbarianCamp::Location::North, 40.0f);

		camps[BarbarianCamp::Location::North] = camp;
	}

	{ // WESTERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, BarbarianCamp::Location::West, 40.0f);

		camps[BarbarianCamp::Location::West] = camp;
	}

	{ // VILLAGE INVADERS
		auto camp = new BarbarianCamp({ -11.5f, 18.0f, -126.0f }, BarbarianCamp::Location::Village, 40.0f, false);

		camp->AddBarbarian("BarbarianBow", { -11.5f, 18.0f, -126.0f }, hostiles, player, CombatStyle::consistantDelay);
		camp->AddBarbarian("BarbarianBow", { -11.5f, 18.0f, -136.0f }, hostiles, player, CombatStyle::consistantDelay);
		camp->AddBarbarian("BarbarianBow", { -11.5f, 18.0f, -146.0f }, hostiles, player, CombatStyle::consistantDelay);

		camps[BarbarianCamp::Location::Village] = camp;
	}
}

void Game::SpawnInvasion()
{
	camps[BarbarianCamp::Location::Village]->Reset();
}

void Game::CheckTargetCollision()
{
	auto handler = player->GetArrowHandler();

	for (auto& arrow : handler.arrows)
	{
		for (auto& target : targets)
		{
			if (target->GotHit())
				continue;

			bool hit = handler.CheckCollision(arrow, target->GetCollider());

			if (hit)
			{
				target->Hit();
				Print("TARGET " + std::to_string(target->GetID()) + " GOT HIT");
				return;
			}
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
	auto NPC = std::make_shared<HostileNPC>(filename, player, combatStyle);
	NPC->SetPosition(position);
	//NPC->BindArrows(modelRenderer);

	auto collider = NPC->GetCollider();
	collider->SetParent(NPC);
	collider->Update();
	collider->SetScale(2, 7, 2);
	CR->Bind(collider);

	MR->Bind(NPC);
	SR->Bind(NPC);
	const std::string name = "hostileNPC" + std::to_string(hostileID);
	scene.AddDrawable(name, NPC);
	hostileID++;
	hostiles.emplace_back(NPC);
}

void Game::AddLoot(LOOTTYPE type, const Vector3& position)
{
	auto LOOT = std::make_shared<Loot>(type, position);
	MR->Bind(LOOT);
	auto collider = LOOT->GetCollider();
	const std::string name = "loot" + std::to_string(lootID);
	LOOT->SetName(name);
	scene.AddDrawable(name, LOOT);
	loot.emplace_back(LOOT);
	lootID++;
	//CR->Bind(LOOT->GetCollider());
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
				//QuestLog::Inst().Save("Test");
				saveStation.LastSave(Time::Get());
			}
		}
	}
}

void Game::CheckItemCollision()
{
	for (auto& item : items)
	{
		if (Collision::Intersection(*item->GetCollider(), *player->GetFrustum()))
		{
			ingameOverlay->ShowInteract();

			if (Event::KeyIsPressed('E'))
			{
				SoundEffect::AddAudio(L"Audio/Pickup.wav", 2);
				SoundEffect::SetVolume(0.005, 2);
				SoundEffect::StartAudio(2);

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
				ingameOverlay->ShowInteract();

				if (Event::KeyIsPressed('E'))
				{
					if (overlay == dialogueOverlay || dialogueOverlay->HasRecentDialogue())
						return;

					overlay = dialogueOverlay;

					SoundEffect::AddAudio(L"Audio/Welcome.wav", 2);
					SoundEffect::SetVolume(0.004, 2);
					SoundEffect::StartAudio(2);

					auto objective = QuestLog::GetTalkObjective(NPC->GetName());
					dialogueOverlay->Set(NPC, (TalkObjective*)objective);
					return;
				}
			}
		}
	}
}
  
void Game::UpdateInventoryUI()
{
	/*canvas->UpdateText("WOOD", std::to_string(player->Inventory().NumOf(RESOURCE::WOOD)));
	canvas->UpdateText("STONE", std::to_string(player->Inventory().NumOf(RESOURCE::STONE)));
	canvas->UpdateText("FOOD", std::to_string(player->Inventory().NumOf(RESOURCE::FOOD)));*/
}

Game::Game(UINT clientWidth, UINT clientHeight, HWND window)
	:water(5000), terrain(2)
{
	RND.InitAnimatedModelRenderer();
	RND.InitColliderRenderer();
	RND.InitModelRenderer();
	RND.InitParticleRenderer();
	RND.InitShadowRenderer();
	RND.InitSkeletonRenderer();
	RND.InitTerrainRenderer();
	RND.InitWaterRenderer();
	RND.InitInteractableRenderer();

	QuestLog::CreateQuests();
	//QuestLog::Load("Default");

	//LOAD SCENE
	Initialize();

	//SET SCENE CAMERA + DIRECTIONAL LIGHT
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, { 1, 1, 1, 1 }, 4, 4);

	//OVERLAYS
	ingameOverlay = new InGameOverlay();
	dialogueOverlay = new DialogueOverlay();
	pauseOverlay = new PauseOverlay();

	overlay = ingameOverlay;

	//PLAYER
	UINT maxArrows = 5;
	player = std::make_shared<Player>(file, scene.GetCamera(), maxArrows);
	player->SetPosition(-75, 20, -630);
	//scene.AddModel("Player", player);
	player->GetBounds()->SetParent(player);
	CR->Bind(player->GetBounds());
	SKR->Bind(player);
	AMR->Bind(player);

	CR->Bind(player->GetFrustum());
	player->GetFrustum()->SetParent(player);
	CR->Bind(scene.GetCamera()->GetCamRay());

	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNames[] = { "BuildingZero", "BuildingFirst", "BuildingSecond" };
	std::string materialNames[] = { "FarmHouse", "FarmHouse", "FarmHouse" };
	building = std::make_shared<Building>(meshNames, materialNames, "Building", Vector3{ -107.5f, 20.0f, -608.5f }, scene);
	building->SetRotation(0, -DirectX::XM_PI, 0);
	building->SetScale(5.85);

	scene.AddModel("Building", building);
	MR->Bind(building);
	SR->Bind(building);

	//ITEMS
	AddItem(Item::Type::Stick, { -134, 22, -594 });
	AddItem(Item::Type::Stick, { -113, 22, -582 });
	AddItem(Item::Type::Stick, { -116, 20, -609 });
	AddItem(Item::Type::Stick, { -91, 20, -593 });
	AddItem(Item::Type::Stick, { -85, 20, -608 });

	//FRIENDLY NPCS
	AddFriendlyNPCs();

	//BARBARIAN CAMPS
	AddBarbarianCamps();

	//TARGETS
	AddTarget("TargetDummy", { -150, 23, -600 }, { 0,0,0 });
	AddTarget("TargetDummy", { -170, 23, -600 }, { 0,0,0 });
	AddTarget("TargetDummy", { -190, 23, -600 }, { 0,0,0 });

	//PARTICLE SYSTEM
	auto campFireSystem = std::make_shared<ParticleSystem>("fire.ps");
	scene.AddParticleSystem("CampfireSystem", campFireSystem, Vector3{ 38.0f, 20.3f, -574.5f });
	PR->Bind(campFireSystem);
	
	//AUDIO
	Audio::AddAudio(L"Audio/Sonrie.wav", 0);
	Audio::SetVolume(0.005, 0);
	//Audio::SetVolume(0.1, 0);
	Audio::StartAudio(0);
	
	(void)Run();
}

Game::~Game()
{
	QuestLog::ShutDown();
	delete quadTree;
	scene.Clear();
	Resources::Inst().Clear();
}

APPSTATE Game::Run()
{
	switch (overlay->Update())
	{
	case OVERLAYSTATE::NO_CHANGE:
		break;

	case OVERLAYSTATE::MAIN_MENU:
		return APPSTATE::MAIN_MENU;

	case OVERLAYSTATE::PAUSE:
	{
		state = GameState::PAUSED;
		overlay = pauseOverlay;
		overlay->ShowCursor();
		break;
	}

	case OVERLAYSTATE::RETURN:
	{
		state = GameState::ACTIVE;
		overlay = ingameOverlay;
		overlay->HideCursor();
		break;
	}
	}

	if (state != GameState::PAUSED)
		Update();

	Render();

	static float lastClick = 0;

	if (Time::Get() - lastClick > 0.5f)
	{
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
			hostiles[0]->GetArrowHandler().ClearArrows();
			CR->Unbind(hostiles[0]->GetCollider());
			MR->Unbind(hostiles[0]);
			scene.DeleteDrawable(hostiles[0]->GetName());
			hostiles[0] = hostiles[hostiles.size() - 1];
			hostiles.resize(hostiles.size() - 1);
			lastClick = Time::Get();
		}

		if (Event::KeyIsPressed(79))
		{
			Audio::StopEngine();
		}
	}

	if (Event::KeyIsPressed('P'))
	{
		PrintVector3("PLAYER POSITION: ", player->GetPosition());
	}

	if (Event::KeyIsPressed('L'))
		player->Inventory().AddItem(Item::Type::Hammer);

	UpdateInventoryUI();

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
		ingameOverlay->UpdateFPS(frames);
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
		hostiles[i]->Update(player);

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
					hostiles[i]->GetArrowHandler().ClearArrows();
					CR->Unbind(hostiles[i]->GetCollider());
					MR->Unbind(hostiles[i]);
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
