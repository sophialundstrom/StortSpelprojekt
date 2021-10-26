#include "GameState.h"

#include "Event.h"
#include "FBXLoader.h"
#include "GameLoader.h"

void GameState::Update()
{
	player->Update(terrain.GetHeightMap());

	QuestLog::Inst().Update();

	scene.Update();

	CheckItemCollision();
	CheckSaveStationCollision();

	scene.UpdateDirectionalLight(player->GetPosition());

	Event::ClearRawDelta();
}

void GameState::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	animatedModelRenderer.Render();

	colliderRenderer.Render();

	terrainRenderer.Render(terrain);

	skeletonRenderer.Render();

	Graphics::Inst().BeginFrame();

	canvas->Render();

	Graphics::Inst().EndFrame();

	Resources::Inst().Reset();
}

void GameState::Pause()
{
	paused = true;
	canvas = canvases["PAUSED"];
}

void GameState::Resume()
{
	paused = false;
	canvas = canvases["INGAME"];
}

void GameState::Options()
{

}

void GameState::HowToPlay()
{
}

void GameState::BackToPause()
{
}

void GameState::MainMenu()
{
}

void GameState::RemoveItem(const std::string& name)
{
	for (UINT i = 0; i < items.size(); ++i)
	{
		if (items[i]->GetName() == name)
		{
			auto item = scene.Get<Item>(name);
			modelRenderer.Unbind(item);
			shadowRenderer.Unbind(item);
			colliderRenderer.Unbind(item->GetBounds());
			items.erase(items.begin() + i);
			scene.DeleteDrawable(name);
			return;
		}
	}
}

void GameState::AddItem(RESOURCE resource, Vector3 position)
{
	auto item = std::make_shared<Item>(resource, Item::Names[resource]);
	auto bounds = item->GetBounds();

	scene.AddDrawable(item->GetName(), item);
	items.emplace_back(item);
	bounds->SetParent(item);
	item->SetPosition(position);
	bounds->Update();
	modelRenderer.Bind(item);
	shadowRenderer.Bind(item);
	colliderRenderer.Bind(bounds);
}

void GameState::AddArrows(UINT amount)
{

}

void GameState::CheckSaveStationCollision()
{
}

void GameState::CheckItemCollision()
{
}

void GameState::AddBuilding(const std::string& name, const std::string fileNames[])
{
	//auto building = std::make_shared<Building>(fileNames, fileNames, name, Vector3{ 0,0,0 });
}

void GameState::UnbindBuildingEffect(std::unique_ptr<BuildingEffect> effect)
{
}

void GameState::UpdateInventoryUI()
{
}

void GameState::Initialize()
{
}

GameState::GameState(UINT clientWidth, UINT clientHeight, HWND window)
	:deferredRenderer(clientWidth, clientHeight),
	animatedModelRenderer(DEFERRED, true),
	modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED),
	colliderRenderer(DEFERRED),
	terrainRenderer(DEFERRED)
{
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 10000.0f, 0.25f, 15.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, { 0, 1, 0 });
	scene.SetDirectionalLight(50, 4, 4);

	//----UI----
	ui = std::make_unique<UI>(window);

	auto ingameCanvas = new Canvas();
	ingameCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight }, "TestImage", "CompassBase.png");
	ingameCanvas->AddImage({ 250, 250 }, "QuestBorder", "QuestBorder.png");
	ingameCanvas->AddText({ 200, 40 }, "AC", "Active Quests", 200, 20, UI::COLOR::GRAY, UI::TEXTFORMAT::TITLE);
	ingameCanvas->AddImage({ clientWidth - 200.0f, 70 }, "Resources", "Resources.png", 0.8);
	ingameCanvas->AddText({ clientWidth - 302.0f, 70 }, "Wood", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);
	ingameCanvas->AddText({ clientWidth - 192.0f, 70 }, "Stone", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);
	ingameCanvas->AddText({ clientWidth - 82.0f, 70 }, "Food", "0", 30, 15, UI::COLOR::GRAY, UI::TEXTFORMAT::DEFAULT);

	for (UINT i = 0; i < 10; ++i)
		ingameCanvas->AddImage({ 50.0f + 50 * i, clientHeight - 40.0f }, "hp" + std::to_string(i), "Heart.png");

	canvases["INGAME"] = ingameCanvas;
	canvas = ingameCanvas;

	//PLAYER
}

GameState::~GameState()
{
}

APPSTATE GameState::Run()
{
	return APPSTATE();
}
