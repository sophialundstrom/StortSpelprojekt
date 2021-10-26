#pragma once
#include "StateENUM.h"
#include "Scene.h"
#include "ShaderData.h"

#include <map>

class AppState 
{
protected:
    Scene scene;
public:
    AppState() = default;
    virtual ~AppState() = default;

    void Delete() { delete this; };

    virtual State Run() = 0;
};