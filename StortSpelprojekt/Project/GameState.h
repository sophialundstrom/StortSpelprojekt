#pragma once
#include "StateENUM.h"
#include "Scene.h"
#include "ShaderData.h"

#include <map>

class GameState 
{
protected:
    Scene scene;
public:
    GameState() = default;
    virtual ~GameState() = default;

    void Delete() { delete this; };

    virtual State Run() = 0;
};