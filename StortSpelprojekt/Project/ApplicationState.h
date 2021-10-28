#pragma once
#include "Scene.h"
#include "ShaderData.h"

#include <map>

enum class APPSTATE { NO_CHANGE, MAIN_MENU, GAME, LEVEL, PARTICLE, EXIT };

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