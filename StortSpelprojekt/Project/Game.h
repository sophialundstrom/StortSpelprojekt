#pragma once
#include "GameState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"
#include "TerrainRenderer.h"


// The state subclass for the level/game
class Game : public GameState
{
private:
    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;
    TerrainRenderer terrainRenderer;



    void Update();
    void Render();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight);
    ~Game(); // Removes drawables and resources

    Terrain terrain;

    // Inherited via GameState
    virtual State Run() override;
};