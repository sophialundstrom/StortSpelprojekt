#pragma once
#include "ApplicationState.h"
#include "ParticleRenderer.h"
#include "ModelRenderer.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"

class MainMenu : public ApplicationState
{
private:
	Canvas* currentCanvas;

	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;

public:

	MainMenu() = default;
	MainMenu(UINT clientWidth, UINT clientHeight, HWND window);
	~MainMenu();

	void Initialize();


	virtual APPSTATE Run() override;

};