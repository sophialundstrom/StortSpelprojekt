#pragma once
#include "Overlay.h"

class PauseOverlay : public Overlay
{
private:
	//BECAUSE PAUSEOVERLAY INHERITS FROM CANVAS 
	//IT CAN HAVE A BASE THAT ALWAYS RENDERS 
	//AND THEN CANVASES WITHIN THAT WE CAN SWAP BETWEEN

	std::map<std::string, Canvas*> canvases;
public:
	PauseOverlay();
	~PauseOverlay();

	// Inherited via Overlay
	virtual void Render() override;
	virtual OVERLAYSTATE Update() override;
};