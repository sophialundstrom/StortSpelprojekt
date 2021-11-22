#pragma once
#include "Canvas.h"
#include "NPCFriendly.h"
#include "TalkObjective.h"

class DialogueOverlay : public Canvas
{
private:
	std::shared_ptr<FriendlyNPC> NPC;
	Objective* objective;

	Text* dialogueName;
	Text* dialogueText;

	float speed = 0;
	const float holdSpeed = 100;
	const float regularSpeed = 10;
	const float timeBetweenChars = 0.2f;
	float timeSinceChar = 0.0f;
	UINT numCharacters = 0;

	float lastInteraction = 0.0f;

	float delay = 0.0f;
	const float doneDelay = 2.0f;
	bool done = true;
public:
	DialogueOverlay();
	~DialogueOverlay();

	void Update();
	void Render();

	bool IsDone()	{ return done; }

	void Set(std::shared_ptr<FriendlyNPC> NPC, TalkObjective* objective = nullptr);
};