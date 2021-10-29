#pragma once

#include "ApplicationState.h"

#include "AnimatedModelRenderer.h"
#include "ParticleRenderer.h"
#include "DeferredRenderer.h"
#include "ColliderRenderer.h"
#include "SkeletonRenderer.h"
#include "TerrainRenderer.h"
#include "ShadowRenderer.h"
#include "ModelRenderer.h"

#include "Building.h"
#include "QuestLog.h"
#include "SaveStation.h"
#include "Canvas.h"
#include "Time.h"

class GameState : public ApplicationState
{
private:
	bool paused = false;

	std::unique_ptr<QuestLog> questLog;
	std::unique_ptr<UI> ui;

	//RENDERERS
	AnimatedModelRenderer animatedModelRenderer;
	ParticleRenderer particleRenderer;
	DeferredRenderer deferredRenderer;
	ColliderRenderer colliderRenderer;
	SkeletonRenderer skeletonRenderer;
	TerrainRenderer terrainRenderer;
	ShadowRenderer shadowRenderer;
	ModelRenderer modelRenderer;

	//SAVE STATIONS
	SaveStation saveStations[2];

	//PLAYER
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Arrow>> arrows;
	std::vector<std::shared_ptr<Arrow>> hostileArrows;

	//ITEMS
	std::vector<std::shared_ptr<Item>> items;

	//BUILDINGS
	std::shared_ptr<Building> building;

	//TERRAIN
	Terrain terrain;

	//CANVASES
	Canvas* canvas;
	std::map<std::string, Canvas*> canvases;

	//----FUNCTIONS----
	void Update();
	void Render();

	//UI
	void Pause();
	void Resume();
	void Options();
	void HowToPlay();
	void BackToPause();
	void MainMenu();

	//ITEMS
	void RemoveItem(const std::string& name);
	void AddItem(RESOURCE resource, Vector3 position);
	
	//ARROWS
	void AddArrows(UINT amount);

	//COLLISION
	void CheckSaveStationCollision();
	void CheckItemCollision();

	//BUILDINGS
	void AddBuilding(const std::string& name, const std::string fileNames[]);
	void UnbindBuildingEffect(std::unique_ptr<BuildingEffect> effect);
	void UpdateInventoryUI();

	void Initialize();
public:
	GameState(UINT clientWidth, UINT clientHeight, HWND window);
	~GameState();

	virtual APPSTATE Run() override;
};