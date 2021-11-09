#pragma once
#include "Canvas.h"

class DialogueOverlay : public Canvas
{
private:
	Text* dialogueName;
	Text* dialogueText;

	float speed = 0;
	const float holdSpeed = 50;
	const float regularSpeed = 10;
	UINT numCharacters = 0;

	const float doneDelay = 0.5f;

	bool active = false;
	bool done = false;
public:
	DialogueOverlay();
	~DialogueOverlay();

	void Update();
	void Render();

	bool IsDone()	{ return done; }
	bool IsActive() { return active; }

	void Set(const std::string& NPCName, const std::string& text);
};