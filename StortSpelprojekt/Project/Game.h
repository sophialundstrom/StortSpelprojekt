#pragma once
#include "GameState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"

#include "Time.h"

//PlayerClassLib
#include <math.h>

// The state subclass for the level/game
class Game : public GameState
{
private:
    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
    ShadowRenderer shadowRenderer;
    DeferredRenderer deferredRenderer;

    //Player Variables and function(s)//TODO: MAKE THIS INTO A PLAYER CLASS!!!

    float playerMoveSpeed = 4;
    float heightMapGroundLevel;
    float mouseSensitivity = 10.f;
    float camDistance = 10;//How far the camera is from the playerboject

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
    Game(UINT clientWidth, UINT clientHeight);
    ~Game(); // Removes drawables and resources

    // Inherited via GameState
    virtual State Run() override;
};