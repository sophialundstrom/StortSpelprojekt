#pragma once
#include "GameState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"
#include "TerrainRenderer.h"
#include "Building.h"
#include "QuestLog.h"
#include "UI.h"
#include "Time.h"


//PlayerClassLib
#include <math.h>

// The state subclass for the level/game
class Game : public GameState
{
private:
    std::unique_ptr<QuestLog> questLog;

    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;
    TerrainRenderer terrainRenderer;

    Terrain terrain;

    std::shared_ptr<Player> player;

    std::shared_ptr<Player> npc;

    std::shared_ptr<Building> building;

    UI userInterface;


    void Update();
    void Render();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};