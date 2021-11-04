#pragma once
#include "UI.h"
#include "GameState.h"
#include "ParticleRenderer.h"
#include "ModelRenderer.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"

class Win : public ApplicationState
{
private:
	Canvas* currentCanvas;
	std::map<std::string, Canvas*> canvases;

	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;
	APPSTATE gameStateStatus = APPSTATE::NO_CHANGE;
	bool goToMenu = false;

public:

	Win() = default;
	Win(UINT clientWidth, UINT clientHeight, HWND window);
	~Win();

	bool quit;
	bool backToMenu;

	void Render();
	void Initialize();
	void MainMenu();
	void QuitGame();

	virtual APPSTATE Run() override;

};