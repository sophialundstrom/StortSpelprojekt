#pragma once
#include "ApplicationState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"
#include "ColliderRenderer.h"
#include "TerrainRenderer.h"
<<<<<<< HEAD
=======
#include "SkeletonRenderer.h"
#include "WaterRenderer.h"
>>>>>>> main
#include "Building.h"
#include "Item.h"
#include "QuestLog.h"
#include "SaveStation.h"
#include "Canvas.h"
#include "Time.h"
#include "Arrow.h"
#include "Audio.h"

//PlayerClassLib
#include <math.h>

// The state subclass for the level/game
class Game : public ApplicationState
{
private:
    bool paused = false;
    const std::string file = "Default"; //"Test"

    std::unique_ptr<QuestLog> questLog;

    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;
    TerrainRenderer terrainRenderer;
    ColliderRenderer colliderRenderer;
<<<<<<< HEAD
=======
    SkeletonRenderer skeletonRenderer;
    WaterRenderer waterRenderer;
>>>>>>> main

    float lastSave = 0;
    SaveStation saveStations[2];

    Terrain terrain;
    Water water;

    Canvas* currentCanvas;
    std::map<std::string, Canvas*> canvases;

    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Arrow>> arrows;

    std::vector<std::shared_ptr <Item>> items;

    std::shared_ptr<Building> building;

    void Update();
    void Render();

    // PAUSE & UI
    void Pause();
    void Resume();
    void Options();
    void HowToPlay();
    void BacktoPause();
    void MainMenu();
  
    bool mainMenu = false;

    void RemoveItem(const std::string name);
    void AddItem(RESOURCE resource, Vector3 position);

    void AddArrow(const std::string fileName);

    void CheckSaveStationCollision();
    void CheckItemCollision();

    void Initialize();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual APPSTATE Run() override;
};