#include "Game.h"
#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "DialogueOverlay.h"
#include "Biome.h"
#include "HardwareSupport.h"

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

	HandleBiomes();

	CheckNearbyEnemies();

	//HandleCamps();

	CheckTargetCollision();

	CheckQuestInteraction();

	UpdateAndHandleLoot();

	HandleAudioSources();

	HandleHouseUpgrades();

	HandleDayNightCycle();

	scene.UpdateDirectionalLight(player->GetPosition());
	//scene.UpdatePointLights();

	QuestLog::Update(player, camps, targets, friendlyNPCs);

	ingameOverlay->UpdateArrowCounter(player->numArrows);
	ingameOverlay->UpdateHealth(player->Stats().healthPoints);
	ingameOverlay->UpdateInventory(player->Inventory());
	ingameOverlay->UpdateQuests(QuestLog::GetActiveQuests());
	UpdateQuadTree();

	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), scene.GetNumberOfPointlights(), scene.GetPointLights());

	Event::ClearRawDelta();
}

void Game::Render()
{
	SR->Render();

	Graphics::Inst().BeginFrame();

	ShaderData::Inst().BindFrameConstants();

	MR->Render();

	AMR->Render();

	//SKR->Render();
	
	SMR->Render();

	//CR->Render();

	IR->Render();

	TR->Render(terrain);

	WR->Render(water);

	SBR->Render();

	PR->Render();

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

void Game::QuitCanvas()
{
	currentCanvas = canvases["QUIT"];
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
			//MR->Bind(model);
			//SR->Bind(model);

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

	pathing = std::make_shared<Pathfinding>();
	pathing->CreateGrid(Vector3::Zero);
	CampData::CampData();
}

void Game::RemoveItem(std::shared_ptr<Item> removedItem)
{
	for (UINT i = 0; i < items.size(); ++i)
	{
		auto& item = items[i];

		if (item == removedItem)
		{
			IR->Unbind(item);
			MR->Unbind(item);
			SR->Unbind(item);
			CR->Unbind(item->GetCollider());

			auto it = items.begin() + i;
			items.erase(it);

			return;
		}
	}
}

void Game::AddItem(Item::Type type, Vector3 position, const Vector3& rotation)
{
	const std::string name = "Item";

	auto item = std::make_shared<Item>(type, name);
	item->SetPosition(position);
	item->SetRotation(rotation);
	auto collider = item->GetCollider();
	collider->SetParent(item);

	items.emplace_back(item);

	MR->Bind(item);
	SR->Bind(item);
	CR->Bind(item->GetCollider());
}

void Game::GenerateRandomItems(const Vector3& center, UINT amount, float radius)
{
	for (UINT i = 0; i < amount; ++i)
	{
		auto r = radius * sqrt(Random::Real(0.0f, 1.0f));
		auto theta = Random::Real(0.0f, 1.0f) * 2.0f * PI;

		auto x = center.x + r * cos(theta);
		auto z = center.z + r * sin(theta);

		auto y = terrain.SampleAverage(x, z);

		auto itemType = (Item::Type)Random::Integer(0, 2);

		Vector3 rotation = { 0.0f, 0.0f, 0.0f };

		switch (itemType)
		{
			case Item::Type::Stick:
			{
				auto x = Random::Real(-PI_DIV4, PI_DIV4);
				auto y = Random::Real(0, 2.0f * PI);
				auto z = Random::Real(-PI_DIV4, PI_DIV4);
				rotation = { x, y, z };
				
				break;
			}

			case Item::Type::Stone: case Item::Type::Food:
			{
				auto x = Random::Real(0, 2.0f * PI);
				auto y = Random::Real(0, 2.0f * PI);
				auto z = Random::Real(0, 2.0f * PI);
				rotation = { x, y, z };

				break;
			}
		}

		AddItem(itemType, { x, y, z }, rotation);
	}
}

std::shared_ptr<FriendlyNPC> Game::AddFriendlyNPC(const std::string& name, const std::string& fileName, Vector3 position)
{
	auto NPC = std::make_shared<FriendlyNPC>(name, fileName);
	NPC->SetPosition(position);

	auto collider = NPC->GetCollider();
	collider->SetParent(NPC);
	collider->Update();
	CR->Bind(collider);

	//MR->Bind(NPC);
	AMR->Bind(NPC);
	SR->Bind(NPC);

	scene.AddDrawable(name, NPC);

	friendlyNPCs.emplace_back(NPC);

	auto marker = NPC->GetQuestMarker();
	//marker->SetParent(NPC);
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
			
			Audio::SetVolume("PickupPop.wav", 1.f);
			Audio::StartEffect("Pickup.wav");
		}
	}
}

void Game::AddFriendlyNPCs()
{
	//Sven
	{
		auto NPC = AddFriendlyNPC("Sven", "Farmer", { -77.253, 20, -588 });
		NPC->SetScale(1.6);
		NPC->SetRotation(0, PI_DIV2, 0);
		
		{
			NPC->AddQuest("Why Don't You Just Axe");
			NPC->AddDialogue("Ah! My friend, the first thing I want you to do is to fetch my old axe. I really need it right now but I lost it in the raid. I think that a barbarian took it and ran into the forest west from here, so I would start looking there. I would go with you, but I'm too scared to do so.");
			NPC->AddDialogue("They probably followed the path from here...");
			NPC->AddDialogue("Thank you! I can finally work again.");
		}
		{
			NPC->AddQuest("Hungry For Apples");
			NPC->AddDialogue("Hi again my friend! I am in need of your help once again. Please bring me some red apples, they are quite frequent in this area.");
			NPC->AddDialogue("Have you eaten all my apples...?");
			NPC->AddDialogue("Thank you so much for bringing me these goodies, i will have a feast tonight.");
		}
		{
			auto quest = NPC->AddQuest("Bye Bye Barbarians");
			NPC->AddDialogue("Hello my best friend! I think you should get rid of all barbarians in that camp to the south to damage their numbers.");
			NPC->AddDialogue("Go back and wipe out the camp to the southwest. I believe you can do it.");
			NPC->AddDialogue("Great work buddy! You really showed them. Maybe we finally can get some peace in the village.");
			auto onActiveFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};
			quest->AddOnActivateFunction(onActiveFunc);
		}

		NPC->AddDialogue("Keep your eyes open, the barbarians are relentless.");
			
		friendlyNPCs.emplace_back(NPC);
	}

	//Ulfric
	{
		auto NPC = AddFriendlyNPC("Ulfric", "BlackSmith", { -18, 18, -677 });
		NPC->SetScale(1.6);
		NPC->SetRotation(0, PI_DIV4, 0);
		{
			NPC->AddQuest("Fetch Me' Hammer");
			NPC->AddDialogue("I need your help retrieving a hammer tha' I need. The last time I had it was when I was venturing into the desert with me old cart. Then the barbarians attacked so I had to leave it there. Please bring it back so that we can start working on rebuilding' the forge.");
			NPC->AddDialogue("Hi again, I thought you would have me hammer by now The cart was not tha' far from the trail leading up into the desert, so tha' should be a good place to start lookin.");
			NPC->AddDialogue("Thanks friend! Tha' hammer really means a lo'to me.");
		}
		{
			NPC->AddQuest("Spy In The Making");
			NPC->AddDialogue("Did you see the camp in the desert? It is kinda suspicious. Would you be able to go an have a look? Just spy on them to see what they're u'to. But be careful, they are quite dangerous.");
			NPC->AddDialogue(" I saw some bushes you should be able to hide in withou' bein' seen to the left o' the camp.");
			NPC->AddDialogue("You're back! How di'it go? I knew they were u'to somethin'!");
		}
		{
			auto quest = NPC->AddQuest("Barbarians No Mo'");
			NPC->AddDialogue("We needo do somethin' abou'it befo' they do somethin' awful.");
			NPC->AddDialogue(" Down with the bastards! Remembe' to stock u' on arrows at Lydia's.");
			NPC->AddDialogue("Wha' a remarkable job! They won' bo'er us now.");
			auto onActiveFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};
			quest->AddOnActivateFunction(onActiveFunc);
		}

		NPC->AddDialogue("Make't sure ye ready fo the barbs. They have a big camp to the north");

		friendlyNPCs.emplace_back(NPC);
	}

	//Lydia
	{
		auto NPC = AddFriendlyNPC("Lydia", "VillageArcherNPC", { 117.5, 18, -655 });
		NPC->SetScale(0.4);
		NPC->SetRotation(0, -PI_DIV2, 0);
		{
			NPC->AddQuest("Getting Started");
			NPC->AddDialogue("Thank the gods that you are alive! The barbarians almost got us all and they have destroyed the village. We're gonna need your help to rebuild it and destroy the barbarians. In case you don't remember me, I'm Lydia and I need you to get something for me. I used to have a archery tent right behind me, but it was destroyed during the raid. I need you to collect some rope so that we can start building a new tent. The last time I saw it, it was laying near the ocean, so if you just explore the beach I think you will find some.");
			NPC->AddDialogue("Back again so soon? I think I remember that the rope was close to the dock.");
			NPC->AddDialogue("Thank you! Now we can finally start rebuilding the tent.");
		}
		{
			NPC->AddQuest("Sticks And Stones");
			NPC->AddDialogue("I will still need some resources for the structure. Please look around for some sticks and rocks that we can use.");
			NPC->AddDialogue("I really need those sticks and rocks right now, it can't be that hard to find");
			NPC->AddDialogue("Great work! Now that wont be enough to rebuild the structure but I will trade you some arrows for it, whenever you are in need of some more arrows, just come back here.");
		}
		{
			auto quest = NPC->AddQuest("Target Aquired");
			NPC->AddDialogue("Now head over there to those target dummies and take a few practice shots.");
			NPC->AddDialogue("Need more arrows? Go to the tent and refill.");
			NPC->AddDialogue("Nice shooting!");
			auto onActiveFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};
			quest->AddOnActivateFunction(onActiveFunc);
		}
		{
			auto quest = NPC->AddQuest("Invasion!");
			NPC->AddDialogue("Oh no, the barbarians are returning! Quick, use your bow and take them down before they kill us all.");
			NPC->AddDialogue("What are you doing?! Help us!");
			NPC->AddDialogue("Amazing! Thanks for saving us.");
			auto onActiveFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};
			quest->AddOnActivateFunction(onActiveFunc);
		}
		{
			NPC->AddQuest("Getting Acquainted");
			NPC->AddDialogue("I think it's time to introduce you to the rest. Go talk to Sven and Ulfric.");
			NPC->AddDialogue("Are you serious? They are right there...");
			NPC->AddDialogue("Thank you! But now it's time to help the others. I think they really need it.");
		}
		{
			auto quest = NPC->AddQuest("Payback");
			NPC->AddDialogue("Hi! I was out searching for raw materials in the woods to craft arrows with and encountered a couple of those damn barbarians. I managed to get a few of them but I can�t take them. Would you help me with the rest of them? I think they came from the eastern camp.");
			NPC->AddDialogue("If the mountains are in due north... And the desert is are in the west... Then the east must be in...?");
			NPC->AddDialogue("Did you get them all? Nice! Now I'll be able to craft all the arrows in the world without interruption!");
			auto onActiveFunc = [this, quest]() mutable
			{
				quest->ResetObjectiveResources(player, camps, targets);
			};
			quest->AddOnActivateFunction(onActiveFunc);
		}

		NPC->AddDialogue("You have helped me so much, I can never repay you for what you have done for me. By the way, when you're ready you should try to clear out the northern camp!");
		friendlyNPCs.emplace_back(NPC);
	}

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
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, CampData::Location::South, 30.0f);
		camp->AddBarbarian("BarbarianAnim", { -582.0f, 86.0f, -273.5f }, hostiles, player, CombatStyle::consistantDelay, { -582.0f, 86.0f, -273.5f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { -572.0f, 87.0f, -213.0f }, hostiles, player, CombatStyle::consistantDelay, { -572.0f, 87.0f, -213.0f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { -670.0f, 81.0f, -208.0f }, hostiles, player, CombatStyle::consistantDelay, { -670.0f, 81.0f, -208.0f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { -671.0f, 67.0f, -240.0f }, hostiles, player, CombatStyle::consistantDelay, { -671.0f, 67.0f, -240.0f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { -671.0f, 67.0f, -256.0f }, hostiles, player, CombatStyle::consistantDelay, { -671.0f, 67.0f, -256.0f }, pathing, false, 1, 3, false);
		camps[CampData::Location::South] = camp;
	}

	{ // EASTERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, CampData::Location::East, 40.0f);
		camp->AddBarbarian("BarbarianAnim", { 597.0f, 77.0f, -461.0f }, hostiles, player, CombatStyle::consistantDelay, { 597.0f, 77.0f, -461.0f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { 650.0f, 80.0f, -516.5f }, hostiles, player, CombatStyle::consistantDelay, { 650.0f, 80.0f, -516.5f }, pathing, false, 1, 3, false);
		camp->AddBarbarian("BarbarianAnim", { 606.0f, 60.0f, -501.5f }, hostiles, player, CombatStyle::consistantDelay, { 606.0f, 60.0f, -501.5f }, pathing, false, 1, 3, false);

		camps[CampData::Location::East] = camp;
	}

	{ // WESTERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, CampData::Location::West, 40.0f);
		camp->AddBarbarian("BarbarianAnim", { -533.5f, 199.0f, 628.0f }, hostiles, player, CombatStyle::consistantDelay, { -533.5f, 199.0f, 628.0f }, pathing, false, 2, 8, false);
		camp->AddBarbarian("BarbarianAnim", { -643.0f, 198.0f, 637.0f }, hostiles, player, CombatStyle::consistantDelay, { -643.0f, 198.0f, 637.0f }, pathing, false, 2, 8, false);
		camp->AddBarbarian("BarbarianAnim", { -574.0f, 172.0f, 578.0f }, hostiles, player, CombatStyle::consistantDelay, { -574.0f, 172.0f, 578.0f }, pathing, false, 2, 8, false);
		camp->AddBarbarian("BarbarianAnim", { -615.0f, 181.0f, 623.0f }, hostiles, player, CombatStyle::consistantDelay, { -615.0f, 181.0f, 623.0f }, pathing, false, 2, 8, false);
		camp->AddBarbarian("BarbarianAnim", { -596.0f, 190.0f, 668.0f }, hostiles, player, CombatStyle::consistantDelay, { -596.0f, 190.0f, 668.0f }, pathing, false, 2, 8, false);

		camps[CampData::Location::West] = camp;
	}

	{ // NORTHERN CAMP
		auto camp = new BarbarianCamp({ 0.0f, 0.0f, 0.0f }, CampData::Location::North, 40.0f);
		camp->AddBarbarian("BarbarianAnim", { 588.3f, 414.5f, 371.0f }, hostiles, player, CombatStyle::consistantDelay, { 588.3f, 414.5f, 371.0f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 654.0f, 414.5f, 472.0f }, hostiles, player, CombatStyle::consistantDelay, { 654.0f, 414.5f, 472.0f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 556.5f, 419.5f, 517.0f }, hostiles, player, CombatStyle::consistantDelay, { 556.5f, 419.5f, 517.0f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 480.5f, 419.5f, 476.5f }, hostiles, player, CombatStyle::consistantDelay, { 480.5f, 419.5f, 476.5f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 571.0f, 400.0f, 413.0f }, hostiles, player, CombatStyle::consistantDelay, { 571.0f, 400.0f, 413.0f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 608.0f, 401.5f, 460.0f }, hostiles, player, CombatStyle::consistantDelay, { 608.0f, 401.5f, 460.0f }, pathing, false, 3, 15, false);
		camp->AddBarbarian("BarbarianAnim", { 536.0f, 402.0f, 472.0f }, hostiles, player, CombatStyle::consistantDelay, { 536.0f, 402.0f, 472.0f }, pathing, false, 3, 15, false);
		camps[CampData::Location::North] = camp;
	}

	{ // VILLAGE INVADERS
		auto camp = new BarbarianCamp({ -11.5f, 18.0f, -126.0f }, CampData::Location::Village, 40.0f, true);

		//camp->AddBarbarian("BarbarianAnim", { -11.5f, 18.0f, -126.0f }, hostiles, player, CombatStyle::consistantDelay, Vector3::Zero);
		//camp->AddBarbarian("BarbarianAnim", { -11.5f, 18.0f, -136.0f }, hostiles, player, CombatStyle::consistantDelay, Vector3::Zero);
		//camp->AddBarbarian("BarbarianAnim", { -11.5f, 18.0f, -116.0f }, hostiles, player, CombatStyle::consistantDelay, Vector3::Zero);

		camps[CampData::Location::Village] = camp;
	}
}

void Game::SpawnInvasion()
{
	//camps[CampData::Location::Village]->Reset();
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { -500, 48, -500 }, hostiles, player, CombatStyle::consistantDelay, { -16.5, 20, -567 }, pathing, false, 3, 15, false);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 7, 56, -398 }, hostiles, player, CombatStyle::consistantDelay, { 2, 20, -579 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 73, 50, -422 }, hostiles, player, CombatStyle::consistantDelay, { 57, 21, -574 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 293, 22, -628 }, hostiles, player, CombatStyle::consistantDelay, { 122, 20, -624 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 243, 24, -576 }, hostiles, player, CombatStyle::consistantDelay, { 71, 20, -626 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 207, 18, -736 }, hostiles, player, CombatStyle::consistantDelay, { 97, 18, -681 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { 200, 10, -791 }, hostiles, player, CombatStyle::consistantDelay, { 47, 18, -675 }, pathing);
	//camps[CampData::Location::Village]->AddBarbarian("BarbarianBow", { -314, 7, -644 }, hostiles, player, CombatStyle::consistantDelay, { -88, 18, -652 }, pathing);
	

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
			if ((arrow->GetPosition() - target->GetPosition()).Length() > 70.f)
				continue;
				
			bool hit = handler.CheckCollision(arrow, target->GetCollider(), player->GetPosition());

			if (hit)
			{
				Audio::StartEffect("ArrowHitDummy.wav");
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
				if ((arrow->GetPosition() - collider->GetPosition()).Length() > 50.f)
					continue;

				hostile->GetArrowHandler().CheckCollision(arrow, collider, player->GetPosition());
			}
		}
		
		for (auto& arrow : player->GetArrowHandler().arrows)
		{
			if (!arrow->canCollide)
				continue;
			if ((arrow->GetPosition() - collider->GetPosition()).Length() > 50.f)
				continue;

			player->GetArrowHandler().CheckCollision(arrow, collider, player->GetPosition());
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

void Game::HandleHouseUpgrades()
{
	for (auto& building : buildings)
	{
		if ((player->GetPosition() - building->GetPosition()).Length() < 170.f)
		{
			if (Collision::Intersection(*building->GetCollider(), *player->GetFrustum()))
			{
				ingameOverlay->ShowInteract();

				if (Event::KeyIsPressed('E') && CheckBuildRequirements(building))
				{
					player->HandleUpgrades(building);
					building->Upgrade();
				}
				else if (Event::KeyIsPressed('E') && !CheckBuildRequirements(building))
				{
					if (building->GetBuildingName() == "ArcherTent")
					{
						if (building->GetCurrentState() == 1)
							player->numArrows = 10;
						if (building->GetCurrentState() == 2)
							player->numArrows = 20;
						if (building->GetCurrentState() == 3)
							player->numArrows = 30;
					}
				}
			}
		}
	}
}

bool Game::CheckBuildRequirements(std::shared_ptr<Building> building)
{
	bool canBuild = false;
	if (building->GetCurrentState() == 1)
	{
		if (player->Inventory().NumOf(Item::Type::Stick) >= building->reqStick1 && player->Inventory().NumOf(Item::Type::Stone) >= building->reqStone1)
		{
			player->Inventory().RemoveItem(Item::Type::Stick, building->reqStick1);
			player->Inventory().RemoveItem(Item::Type::Stone, building->reqStone1);
			canBuild = true;
		}
	}
	if (building->GetCurrentState() == 2)
	{
		if (player->Inventory().NumOf(Item::Type::Stick) >= building->reqStick2 && player->Inventory().NumOf(Item::Type::Stone) >= building->reqStone2)
		{
			player->Inventory().RemoveItem(Item::Type::Stick, building->reqStick2);
			player->Inventory().RemoveItem(Item::Type::Stone, building->reqStone2);
			canBuild = true;
		}
	}

	return canBuild;
}

void Game::CheckItemCollision()
{
	for (auto& item : items)
	{
		if ((player->GetPosition() - item->GetPosition()).Length() < 70.f)
		{
			item->Update();

			if (IR->IsBound(item))
				IR->Unbind(item);

			if (Collision::Intersection(*item->GetCollider(), *player->GetFrustum()))
			{
				ingameOverlay->ShowInteract();
				IR->Bind(item);

				if (Event::KeyIsPressed('E'))
				{
					Audio::StartEffect("Pickup.wav");
					Audio::SetVolume("Pickup.wav", Audio::effectsVolume * 2);
					player->Inventory().AddItem(item->GetType());
					RemoveItem(item);
					UpdateInventoryUI();

					return;
				}
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
			if ((player->GetPosition() - NPC->GetPosition()).Length() < 70.f)
			{
				if (Collision::Intersection(*NPC->GetCollider(), *player->GetFrustum()))
				{
					ingameOverlay->ShowInteract();

					if (Event::KeyIsPressed('E'))
					{
						if (overlay == dialogueOverlay || dialogueOverlay->HasRecentDialogue())
							return;

						overlay = dialogueOverlay;

						auto objective = QuestLog::GetTalkObjective(NPC->GetName());
						dialogueOverlay->Set(NPC, (TalkObjective*)objective);
						return;
					}
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
	:water(5000), terrain(3)
{
	Audio::StartEngine();

	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.05f, 100.0f, { -75.0f, 40.0f, -999.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(500, { 1, 1, 1, 1 }, 4, 4);

	//INIT WHICH RENDERERS WE WANT TO USE
	RND.InitAnimatedModelRenderer();
	RND.InitColliderRenderer();
	RND.InitModelRenderer();
	RND.InitStaticModelRenderer();
	RND.InitParticleRenderer();
	RND.InitShadowRenderer();
	RND.InitSkeletonRenderer();
	RND.InitTerrainRenderer();
	RND.InitWaterRenderer();
	RND.InitInteractableRenderer();
	RND.InitSkyBoxRenderer();

	//CREATE OR LOAD QUESTS
	QuestLog::CreateQuests();
	//QuestLog::Load("Default");

	//LOAD SCENE
	Initialize();

	SetupAudio();

	//SET SCENE CAMERA + DIRECTIONAL LIGHT

	//OVERLAYS
	ingameOverlay = new InGameOverlay();
	dialogueOverlay = new DialogueOverlay();
	pauseOverlay = new PauseOverlay();

	overlay = ingameOverlay;

	//PLAYER
	UINT maxArrows = 5;
	player = std::make_shared<Player>(file, scene.GetCamera(), maxArrows);
	player->SetPosition(-75.0f, 20.0f, -725.0f);
	auto collider = player->GetBounds();
	collider->SetParent(player);
	CR->Bind(collider);
	SKR->Bind(player);
	AMR->Bind(player);

	CR->Bind(player->GetFrustum());
	player->GetFrustum()->SetParent(player);
	CR->Bind(scene.GetCamera()->GetCamRay());

	//BUILDING
	//MESH NAMES MUST BE SAME IN MAYA AND FBX FILE NAME, MATERIAL NAME MUST BE SAME AS IN MAYA
	std::string meshNamesFarm[] = { "BuildingZero", "BuildingFirst", "BuildingSecond" };
	std::string materialNamesFarm[] = { "FarmHouse", "FarmHouse", "FarmHouse" };
	buildings[0] = std::make_shared<Building>(meshNamesFarm, materialNamesFarm, "FarmHouse", Vector3{ -107.5f, 18.0f, -608.5f }, scene, "stableSmoke.ps");
	buildings[0]->SetRotation(0, -DirectX::XM_PI, 0);
	buildings[0]->SetScale(5.85);
	buildings[0]->MoveCollider({ 10, 0, 2 });
	buildings[0]->SetColliderRadius(20.0f);
	buildings[0]->SetRequirements(20,20,40,40);

	std::string meshNamesTent[] = { "ArcherTent1", "ArcherTent2", "ArcherTent3" };
	std::string materialNamesTent[] = { "ArcherTentTexture", "ArcherTentTexture", "ArcherTentTexture" };
	buildings[1] = std::make_shared<Building>(meshNamesTent, materialNamesTent, "ArcherTent", Vector3{ 128.86f, 18.12f, -643.05f }, scene, "tentSmoke.ps");
	buildings[1]->SetRotation(0, -DirectX::XM_PIDIV4, 0);
	buildings[1]->SetScale(1.566);
	buildings[1]->SetColliderRadius(17.0f);
	buildings[1]->SetRequirements(20, 20, 40, 40);

	std::string meshNamesBS[] = { "BSLevel1", "BSLevel2", "BSLevel3" };
	std::string materialNamesBS[] = { "albedoBlacksmith", "albedoBlacksmith", "albedoBlacksmith" };
	buildings[2] = std::make_shared<Building>(meshNamesBS, materialNamesBS, "Blacksmith", Vector3{ -5.4f, 17.86f, -701.5f }, scene, "smithSmoke.ps");
	buildings[2]->SetRotation(0, 0, 0);
	buildings[2]->SetScale(1.776);
	buildings[2]->MoveCollider({ 13, 0, 0 });
	buildings[2]->SetColliderRadius(20.0f);
	buildings[2]->SetRequirements(20, 20, 40, 40);

	scene.AddDrawable("FarmHouse", buildings[0]);
	scene.AddDrawable("ArcherTent", buildings[1]);
	scene.AddDrawable("Blacksmith", buildings[2]);

	for (int i = 0; i < 3; i++)
	{
		MR->Bind(buildings[i]);
		SR->Bind(buildings[i]);
		CR->Bind(buildings[i]->GetCollider());
	}


	//ITEMS
	AddItem(Item::Type::Hammer, { -175.0f, 148.0f, 336.0f });
	AddItem(Item::Type::Rope, { -100.0f, 12.3f, -778.0f });
	AddItem(Item::Type::Axe, { -15.0f, 66.0f, -340.0f });

	//RANDOM ITEMS
	GenerateRandomItems({ 58.0f, 20.3f, -574.5f }, 55, 100);

	//FRIENDLY NPCS
	AddFriendlyNPCs();

	//BARBARIAN CAMPS
	AddBarbarianCamps();

	//TARGETS
	AddTarget("TargetDummy", { -150, 23, -600 }, { 0,0,0 });
	AddTarget("TargetDummy", { -170, 23, -600 }, { 0,0,0 });
	AddTarget("TargetDummy", { -190, 23, -600 }, { 0,0,0 });

	//PARTICLE SYSTEM
	auto campFireSystem = std::make_shared<ParticleSystem>("newFire.ps");
	scene.AddParticleSystem("CampfireSystem", campFireSystem, Vector3{ 38.0f, 20.3f, -574.5f });
	PR->Bind(campFireSystem);
	//scene.AddPointLight({ 0.0f, 30.7f, -554.542f }, 40, { 0.5f, 0.0f, 0.05f }, { 190.0f / 255.0f, 83.0f / 255.0f, 21.0f / 255.0f, 1.0f });
	auto bigCampFireSystem = std::make_shared<ParticleSystem>("largeFire.ps");
	scene.AddParticleSystem("BigCampFireSystem", bigCampFireSystem, Vector3{ 573.2f, 401.5f, 449.0f });
	PR->Bind(bigCampFireSystem);
	auto southFireSystem = std::make_shared<ParticleSystem>("newFire.ps");
	scene.AddParticleSystem("SouthCampfireSystem", southFireSystem, Vector3{ -672.0f, 69.5f, -248.0f });
	PR->Bind(southFireSystem);
	auto westFireSystem = std::make_shared<ParticleSystem>("newFire.ps");
	scene.AddParticleSystem("WestCampfireSystem", westFireSystem, Vector3{ -574.0f, 192.5f, 675.5f });
	PR->Bind(westFireSystem);
	auto eastFireSystem = std::make_shared<ParticleSystem>("newFire.ps");
	scene.AddParticleSystem("EastCampfireSystem", eastFireSystem, Vector3{ 635.0f, 67.0f, -488.0f });
	PR->Bind(eastFireSystem);

	auto mountain = std::make_shared<Biome>(13U, BIOME::MOUNTAIN);
	mountain->AddCollider(Vector3(-294, 108, 978), 534);
	mountain->AddCollider(Vector3(312, 110, 790), 460);
	mountain->AddCollider(Vector3(523, 110, 525), 460);
	mountain->AddCollider(Vector3(708, 110, 456), 460);
	mountain->Bind();
	biomes.emplace_back(mountain);

	auto desert = std::make_shared<Biome>(3U, BIOME::DESERT);
	desert->AddCollider(Vector3(-311, 31, 380), 433);
	desert->AddCollider(Vector3(-39, 35, 258), 286);
	desert->AddCollider(Vector3(-556, 31, 459), 397);
	desert->Bind();
	biomes.emplace_back(desert);

	auto ocean = std::make_shared<Biome>(14U, BIOME::OCEAN);
	ocean->AddCollider(Vector3(444, 18, -1089), 463);
	ocean->AddCollider(Vector3(-46, 18, -1114), 389);
	ocean->AddCollider(Vector3(-264, 18, -852), 280);
	ocean->AddCollider(Vector3(-507, 18, -754), 345);
	ocean->Bind();
	biomes.emplace_back(ocean);

	audioSources.emplace_back(AudioSource(Vector3(38.f, 20.f, -574.f), 60.f, "Fireplace.wav"));

	SpawnInvasion();

	(void)Run();
}

Game::~Game()
{
	RND.ShutDown();
	QuestLog::ShutDown();
	delete quadTree;
	scene.Clear();

	Resources::Inst().Clear();
}

void Game::SetupAudio()
{
	
	Audio::SetMusicVolume(Audio::musicVolume);

	Audio::StartMusic("SoundForest.wav");
	Audio::SetVolume("SoundForest.wav", Audio::musicVolume);

}

void Game::HandleAudioSources()
{
	for (auto& audioSource : audioSources)
	{
		audioSource.CheckActive(player->GetPosition());
	}
}

void Game::HandleCamps()
{

	short int numInCombat = 0;
	int numDead = 0;

	// for each camp
	for (auto& [key, camp] : camps)
	{

		for (int i = 0; i < camp->barbarians.size(); i++)
		{
			float distanceToHostile = (player->GetPosition() - hostiles[i]->GetPosition()).Length();

			camp->barbarians[i]->Update(player, terrain.GetHeightMap());

			camp->barbarians[i]->CheckPlayerCollision(player);

			for (auto& arrow : player->GetArrowHandler().arrows)
			{
				if (!arrow->canCollide)
					continue;

				bool hit = player->GetArrowHandler().CheckCollision(arrow, camp->barbarians[i]->GetCollider(), player->GetPosition(), true);

				if (hit)
				{
					camp->barbarians[i]->TakeDamage(player->Stats().damage);
					Audio::StartEffect("BarbHit1.wav");
					if (camp->barbarians[i]->IsDead())
					{
						Audio::StartEffect("BarbDead.wav");
						camp->barbarians[i]->TakeDamage(player->Stats().damage);
						player->Stats().barbariansKilled++;
						AddLoot(LOOTTYPE::ARROWS, camp->barbarians[i]->GetPosition() + Vector3(0, -3, 0));
						camp->barbarians[i]->GetArrowHandler().ClearArrows();
						CR->Unbind(camp->barbarians[i]->GetCollider());
						MR->Unbind(camp->barbarians[i]);
						SR->Unbind(camp->barbarians[i]);
						camp->barbarians[i] = camp->barbarians[camp->barbarians.size() - 1 - numDead];
						numDead++;
						distanceToHostile = 100000.f;
						break;
					}
				}

			}
			if (distanceToHostile < camp->barbarians[i]->viewDistance) // Should be compared to if the hostile "sees" the player instead of a hardcoded value.
			{
				numInCombat++;

			}
		}
		camp->barbarians.resize(camp->barbarians.size() - numDead);
	}
	// end of loop

	if (!player->inCombat && numInCombat > 0)
	{
		player->inCombat = true;
		short int rand = Random::Integer(0, 2);

		switch (rand)
		{
		case 0:
			Audio::StartMusic("Camelot.wav");
			break;

		case 1:
			Audio::StartMusic("combat1.wav");
			break;

		case 2:
			Audio::StartMusic("combat2.wav");
			break;
		}
	}
	else if (player->inCombat && numInCombat == 0)
	{
		player->inCombat = false;
		player->SwitchBiomeMusic();
	}

}

void Game::HandleDayNightCycle()
{
	worldClockTime += Time::GetDelta();

	if (worldClockTime >= (dayLength + nightLength))
		worldClockTime = (worldClockTime - (dayLength + nightLength));

	if (worldClockTime > dayLength && worldClockTime < (dayLength + nightLength))
	{
		//NIGHT
		if (timeSliderVal > 0)
			timeSliderVal -= (Time::GetDelta() * fadeTimeMultiplier);
		if (timeSliderVal < 0)
			timeSliderVal = 0;
	}
	else
	{
		//DAY
		if (timeSliderVal < 1)
			timeSliderVal += (Time::GetDelta() * fadeTimeMultiplier);
		if (timeSliderVal > 1)
			timeSliderVal = 1;
	}

	lightColor = DayLightColor* timeSliderVal;
	lightColor += NightLightColor * (1 - timeSliderVal);

	//std::cout << worldClockTime << "			" << timeSliderVal << std::endl;
	SBR->PullDayNightSlider(timeSliderVal);
	scene.SetDirectionalLight(500, lightColor, 4, 4);

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
		if (Time::Get() - lastStateChange > 0.25f)
		{
			state = GameState::PAUSED;
			overlay = pauseOverlay;
			overlay->ShowCursor();
			lastStateChange = Time::Get();
		}
	
		break;
	}

	case OVERLAYSTATE::RETURN:
	{
		if (Time::Get() - lastStateChange > 0.25f)
		{
			state = GameState::ACTIVE;
			overlay = ingameOverlay;
			overlay->HideCursor();
			lastStateChange = Time::Get();
		}
	
		break;
	}
	}

	if (state != GameState::PAUSED)
		Update();

	Render();

	static float lastClick = 0;

	if (Time::Get() - lastClick > 0.5f)
	{
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
		if (Event::KeyIsPressed('K'))
		{
			PrintVector3(player->GetPosition());
			lastClick = Time::Get();
		}
		if (Event::KeyIsPressed('H'))
		{
			SpawnInvasion();
			lastClick = Time::Get();
		}

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

void Game::HandleBiomes()
{

	bool hit = false;
	BIOME type = BIOME::DEFAULT; // IF NO BIOME IS HIT -> DEFAULT IS PLAYED
	for (auto& biome : biomes)
	{
		for (auto& collider : biome->colliders)
		{
			hit = Collision::Contains(*collider, player->GetPosition());
			if (hit)
			{
				type = biome->type; // GETS BIOME TYPE
			}
		}
		
	}

	player->currentBiome = type; // CURRENT BIOME SET

	if (player->currentBiome != player->previousBiome) // IF NEW BIOME != LAST BIOME
	{
		if (!player->inCombat)
		{
			player->SwitchBiomeMusic();
		}
	}

	player->previousBiome = player->currentBiome;
}

void Game::CheckNearbyEnemies()
{
	short int numInCombat = 0;
	int numDead = 0;
	if (Event::KeyIsPressed('9'))
	{
		PrintS("");
	}
	for (int i = 0; i < hostiles.size(); i++)
	{
		float distanceToHostile = (player->GetPosition() - hostiles[i]->GetPosition()).Length();

		hostiles[i]->Update(player, terrain.GetHeightMap());

		hostiles[i]->CheckPlayerCollision(player);

		for (auto& arrow : player->GetArrowHandler().arrows)
		{
			if (!arrow->canCollide)
				continue;

			bool hit = player->GetArrowHandler().CheckCollision(arrow, hostiles[i]->GetCollider(), player->GetPosition(), true);

			if (hit)
			{

				hostiles[i]->TakeDamage(player->Stats().damage);
				Audio::StartEffect("BarbHit1.wav");
				if (hostiles[i]->IsDead())
				{
					Audio::StartEffect("BarbDead.wav");
					hostiles[i]->TakeDamage(player->Stats().damage);

					player->Stats().barbariansKilled++;
					AddLoot(LOOTTYPE::ARROWS, hostiles[i]->GetPosition() + Vector3(0, -3, 0));
					hostiles[i]->GetArrowHandler().ClearArrows();
					CR->Unbind(hostiles[i]->GetCollider());
					AMR->Unbind(hostiles[i]);
					//MR->Unbind(hostiles[i]);
					SR->Unbind(hostiles[i]);
					hostiles[i] = hostiles[hostiles.size() - 1 - numDead];
					numDead++;
					distanceToHostile = 100000.f;
					break;
				}
			}

		}
		if (distanceToHostile < hostiles[i]->viewDistance) // Should be compared to if the hostile "sees" the player instead of a hardcoded value.
		{
			numInCombat++;

		}
	}
	hostiles.resize(hostiles.size() - numDead);

	if (!player->inCombat && numInCombat > 0)
	{
		//PrintS("IN COMBAT");

		player->inCombat = true;
		short int rand = Random::Integer(0, 2);

		switch (rand)
		{
		case 0:
			Audio::StartMusic("combat1.wav");
			break;

		case 1:
			Audio::StartMusic("combat1.wav");
			break;

		case 2:
			Audio::StartMusic("combat2.wav");
			break;
		}
	}
	else if (player->inCombat && numInCombat == 0)
	{
		//PrintS("OUT OF COMBAT"); 

		{

			player->inCombat = false;
			player->SwitchBiomeMusic();
		}
	}
}

//void Game::HoveringBackHowToPlay()
//{
//	canvases["HOW TO PLAY"]->GetImage("BackLeavesHowToPlay")->Show();
//}
//
//void Game::HoveringBackOptions()
//{
//	canvases["OPTIONS"]->GetImage("BackLeavesHowToPlay")->Show();
//}
//
//void Game::HoveringYes()
//{
//	canvases["QUIT"]->GetImage("YesLeaves")->Show();
//
//}
//
//void Game::HoveringNo()
//{
//	canvases["QUIT"]->GetImage("NoLeaves")->Show();
//
//}
//
//void Game::HoveringOptions()
//{
//	canvases["PAUSED"]->GetImage("OptionsLeaves")->Show();
//}
//
//void Game::HoveringResume()
//{
//	canvases["PAUSED"]->GetImage("ResumeLeaves")->Show();
//}
//
//void Game::HoveringHowToPlay()
//{
//	canvases["PAUSED"]->GetImage("HowToPlayLeaves")->Show();
//}
//
//void Game::HoveringMainMenu()
//{
//	canvases["PAUSED"]->GetImage("BackToMainMenuLeaves")->Show();
//}

void Game::UpdateQuadTree()
{
	drawablesToBeRendered.clear();
	SMR->Clear();
	SR->ClearStatic();


	frustrumCollider.Update(scene.GetCamera());
	quadTree->CheckModelsWithinView(drawablesToBeRendered, frustrumCollider);

	for (auto& [name, drawable] : drawablesToBeRendered)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			SMR->Bind(drawable);
		}
	}

	orthographicCollider.Update(scene.GetDirectionalLight());
	quadTree->CheckModelsWithinView(drawablesToBeRendered, orthographicCollider);

	for (auto& [name, drawable] : drawablesToBeRendered)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			SR->BindStatic(drawable);
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
