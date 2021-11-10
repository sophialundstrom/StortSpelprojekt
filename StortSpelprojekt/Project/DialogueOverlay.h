#pragma once
#include "Canvas.h"

class DialogueOverlay : public Canvas
{
private:
	Text* dialogueName;
	Text* dialogueText;

	float speed = 0;
	const float holdSpeed = 100;
	const float regularSpeed = 10;
	const float timeBetweenChars = 0.2f;
	float timeSinceChar = 0.0f;
	UINT numCharacters = 0;

	float delay = 0.0f;
	const float doneDelay = 2.0f;
	bool done = false;
public:
	DialogueOverlay();
	~DialogueOverlay();

	void Update();
	void Render();

	bool IsDone()	{ return done; }

	void Set(const std::string& NPCName, const std::string& text);
};