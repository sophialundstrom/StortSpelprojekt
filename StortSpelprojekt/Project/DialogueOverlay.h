#pragma once
#include "Overlay.h"
#include "NPCFriendly.h"
#include "TalkObjective.h"

class DialogueOverlay : public Overlay
{
private:
	std::shared_ptr<FriendlyNPC> NPC;
	Objective* objective;

	Text* dialogueName;
	Text* dialogueText;

	float speed = 0;
	const float holdSpeed = 1000;
	const float regularSpeed = 5;
	const float timeBetweenChars = 0.02f;
	float timeSinceChar = 0.0f;
	UINT numCharacters = 0;

	float lastInteraction = 0.0f;

	float delay = 0.0f;
	const float doneDelay = 1.5f;
	bool done = true;
public:
	DialogueOverlay();
	~DialogueOverlay();

	bool HasRecentDialogue() { return (Time::Get() - lastInteraction < 1.0f); }
	void Set(std::shared_ptr<FriendlyNPC> NPC, TalkObjective* objective = nullptr);

	// Inherited via Overlay
	virtual void Render() override;
	virtual OVERLAYSTATE Update() override;
};