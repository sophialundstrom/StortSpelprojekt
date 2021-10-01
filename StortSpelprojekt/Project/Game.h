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
#include "NPCBase.h"

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

    //Player Variables and function(s)//TODO: MAKE THIS INTO A PLAYER CLASS!!!
    float movementOfsetRadiant = 0;
    float playerMoveSpeed = 4;
    float heightMapGroundLevel = 5;
    float mouseSensitivity = 10.f;
    float camDistance = 10;//How far the camera is from the playerboject

    float gravity = 9.82f;
    float timePassed = 0;

    float maxJumpHeight = 1;
    float jumpVelocity = 0;
    float playerVelocity = 0;
    float airTime = 0;

    float get2dAngle(Vector2 a, Vector2 b)
    {
        //MathExplanation
        //https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors

        a.Normalize();
        b.Normalize();

        return acos(a.x * b.x + a.y * b.y);
    };

    //Player Varaibles and function(s) over

    void Update();
    void Render();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};