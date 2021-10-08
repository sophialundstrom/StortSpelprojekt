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
#include "UI.h"
#include "Time.h"

//PlayerClassLib
#include <math.h>

// The state subclass for the level/game
class Game : public GameState
{
private:
    bool gameIsRunning = true;
    const std::string file = "Default"; //"Test"

    std::unique_ptr<QuestLog> questLog;

    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;
    TerrainRenderer terrainRenderer;
    ColliderRenderer colliderRenderer;

    Terrain terrain;

    std::shared_ptr<Player> player;

    std::vector<std::shared_ptr <Item>> items;

    std::shared_ptr<Building> building;

    UI userInterface;

    void Update();
    void Render();

    void RemoveItem(const std::string name);
    void AddItem(RESOURCE resource, Vector3 position);

    void CheckItemCollision();

    void Initialize();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};