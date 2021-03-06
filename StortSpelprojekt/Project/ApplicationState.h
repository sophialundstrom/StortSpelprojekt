#pragma once
#include "Scene.h"
#include "ShaderData.h"
#include "Audio.h"
#include <map>

enum class APPSTATE { NO_CHANGE, MAIN_MENU, GAME, LEVEL, PARTICLE, EXIT, GAMEOVER, WIN};

class ApplicationState 
{
protected:
    Scene scene;
public:
    ApplicationState() = default;
    virtual ~ApplicationState() = default;

    void Delete() { delete this; };

    virtual APPSTATE Run() = 0;
};