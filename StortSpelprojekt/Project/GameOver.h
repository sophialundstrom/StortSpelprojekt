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

class GameOver : public ApplicationState
{
private:
	Canvas* currentCanvas;
	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;
	std::unique_ptr<UI> ui;
	APPSTATE gameStateStatus = APPSTATE::NO_CHANGE;
	bool goToMenu = false;
public:

	GameOver() = default;
	GameOver(UINT clientWidth, UINT clientHeight, HWND window);
	~GameOver();

	void Initialize();
	void MainMenu();
	void QuitGame();

	virtual APPSTATE Run() override;

};