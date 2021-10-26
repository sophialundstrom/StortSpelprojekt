#pragma once
#include "AppStates.h"
#include "Scene.h"

class ApplicationState
{
protected:
	Scene scene;
public:
	ApplicationState() = default;
	virtual ~ApplicationState() = default;
	virtual APPSTATE Run() = 0;
};