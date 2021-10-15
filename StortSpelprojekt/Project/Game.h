#pragma once
#include "GameState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"
#include "ColliderRenderer.h"
#include "TerrainRenderer.h"
#include "Building.h"
#include "Item.h"
#include "QuestLog.h"
#include "SaveStation.h"
#include "Canvas.h"
#include "Time.h"

//PlayerClassLib
#include <math.h>

// The state subclass for the level/game
class Game : public GameState
{
private:
    bool paused = false;
    const std::string file = "Default"; //"Test"

    std::unique_ptr<QuestLog> questLog;
    std::unique_ptr<UI> userInterface;

    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;
    TerrainRenderer terrainRenderer;
    ColliderRenderer colliderRenderer;

    float lastSave = 0;
    SaveStation saveStations[2];

    Terrain terrain;

    Canvas* currentCanvas;
    std::map<std::string, Canvas*> canvases;

    std::shared_ptr<Player> player;

    std::vector<std::shared_ptr <Item>> items;

    std::shared_ptr<Building> building;

    void Update();
    void Render();

    void Pause();
    void Resume();

    void RemoveItem(const std::string name);
    void AddItem(RESOURCE resource, Vector3 position);

    void CheckSaveStationCollision();
    void CheckItemCollision();

    void Initialize();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};