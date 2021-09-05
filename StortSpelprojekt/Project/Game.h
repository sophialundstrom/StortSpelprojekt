#pragma once
#include "GameState.h"
#include "RenderGraph.h"

// The state subclass for the level/game
class Game : public GameState
{
private:

public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};