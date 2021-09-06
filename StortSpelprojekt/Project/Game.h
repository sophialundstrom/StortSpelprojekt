#pragma once
#include "GameState.h"
#include "RenderGraph.h"

// The state subclass for the level/game
class Game : public GameState
{
private:
    ParticleRenderer<Deferred> particleRenderer;
    ModelRenderer<Deferred, true> modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;

    void Update();
    void Render();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};